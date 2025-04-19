#define _GNU_SOURCE
#include "config.h"
#include "command_line.h"
#include "frontier.h"
#include "graph.h"
#include "merged_csr.h"
#include "mt19937-64.h"
#include "pthread_barrier.h"
#include "thread_pool.h"
#include "debug_utils.h"
#include <pthread.h>
#include <assert.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>

GraphCSR *graph;
Frontier *f1, *f2;
uint32_t *distances;

pthread_barrier_t barrier;
atomic_int active_threads;

thread_pool_t tp;

volatile uint32_t exploration_done;
pthread_cond_t bfs_done_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t bfs_done_mutex = PTHREAD_MUTEX_INITIALIZER;
uint32_t bfs_done;

void top_down_chunk(GraphCSR *graph, Frontier *next, VertexChunk *c,
                    VertexChunk **dest, int distance, int thread_id) {
  assert(c != NULL && "Chunk passed to top_down_chunk is NULL!");
  uint32_t v = UINT32_MAX;
  while ((v = remove_vertex(c)) != UINT32_MAX) {
    uint32_t end = v + DEGREE(graph, v) + METADATA_SIZE;
    for (uint32_t i = v + METADATA_SIZE; i < end; i++) {
      uint32_t neighbor = graph->col_idx[i];
      if (DISTANCE(graph, neighbor) == UINT32_MAX) {
        DISTANCE(graph, neighbor) = distance;
        if (DEGREE(graph, neighbor) != 1) {
          if (*dest == NULL || (*dest)->next_free_index >= CHUNK_SIZE) {
            *dest = create_chunk(next, thread_id);
          }
          insert_vertex(*dest, neighbor);
        }
      }
    }
  }
}

void top_down(GraphCSR *graph, Frontier *current, Frontier *next, int distance,
              int thread_id) {
  VertexChunk *c = NULL;
  VertexChunk *next_chunk = NULL;
  VertexChunk **dest = &next_chunk;
  while ((c = remove_chunk(current, thread_id)) != NULL) {
    top_down_chunk(graph, next, c, dest, distance, thread_id);
  }
  // Work stealing from other threads when finished processing chunks of this
  // thread
  uint32_t work_to_do = 1;
  while (work_to_do) {
    work_to_do = 0;
    for (int i = 0; i < MAX_THREADS; i++) {
      if (current->chunk_counts[i] > 1) {
        work_to_do = 1;
        if ((c = remove_chunk(current, i)) != NULL) {
          top_down_chunk(graph, next, c, dest, distance, thread_id);
        }
        i--;
      }
    }
  }
}

void finalize_distances(int thread_id) {
  // Write distances from mergedCSR to distances array
  uint32_t chunk_size = graph->num_vertices / MAX_THREADS;
  uint32_t start = thread_id * chunk_size;
  uint32_t end = (thread_id == MAX_THREADS - 1) ? graph->num_vertices
                                                : (thread_id + 1) * chunk_size;
  for (uint32_t i = start; i < end; i++) {
    distances[i] = DISTANCE(graph, graph->row_ptr[i]);
    DISTANCE(graph, graph->row_ptr[i]) = UINT32_MAX;
  }
}

void *thread_main(void *arg) {
  int thread_id = *(int *)arg;
  while (wait_for_work(&tp) == 0) {
    int distance = 0;
    while (!exploration_done) {
      top_down(graph, f1, f2, distance, thread_id);
      if (atomic_fetch_sub(&active_threads, 1) == 1) {
        // Swap frontiers
        Frontier *temp = f2;
        f2 = f1;
        f1 = temp;
        active_threads = MAX_THREADS;
        if (get_total_chunks(f1) == 0)
          exploration_done = 1;
        // print_chunk_counts(f1);
        // printf("Done distance %d\n", distance);
      }
      pthread_barrier_wait(&barrier);
      distance++;
    }
    finalize_distances(thread_id);
    
    if (atomic_fetch_sub(&active_threads, 1) == 1) {
      pthread_mutex_lock(&bfs_done_mutex);
      bfs_done = 1;
      pthread_cond_signal(&bfs_done_cond);
      pthread_mutex_unlock(&bfs_done_mutex);
    }
  }
  return NULL;
}

void initialize_bfs() {
  to_merged_csr(graph);
  f1 = init_frontier();
  f2 = init_frontier();
  // Initialize barriers
  if (pthread_barrier_init(&barrier, NULL, MAX_THREADS) != 0) {
    perror("Could not initialize the barrier");
    exit(1);
  }
  tp = init_thread_pool();
  create_threads(&tp, thread_main);
}

void bfs(uint32_t source) {
  // Convert source vertex to mergedCSR index
  source = graph->row_ptr[source];
  DISTANCE(graph, source) = 0;
  VertexChunk *c = create_chunk(f1, 0);
  insert_vertex(c, source);
  exploration_done = 0;
  active_threads = MAX_THREADS;

  notify_threads(&tp, 0);

  pthread_mutex_lock(&bfs_done_mutex);
  bfs_done = 0;
  while (!bfs_done) {
    pthread_cond_wait(&bfs_done_cond, &bfs_done_mutex);
  }
  pthread_mutex_unlock(&bfs_done_mutex);
}

void print_time(double elapsed[], int runs) {
  double total = 0;
  for (int i = 0; i < runs; i++) {
    total += elapsed[i];
  }
  printf("Average time: %14.5f\n", total / runs);
}

uint32_t *generate_sources(int runs, uint32_t num_vertices, uint32_t source) {
  uint32_t *sources = malloc(runs * sizeof(uint32_t));
  if (source != UINT32_MAX) {
    for (int i = 0; i < runs; i++) {
      sources[i] = source;
    }
  } else {
    init_genrand64(SEED);
    for (int i = 0; i < runs; i++) {
      do {
        uint64_t gen = genrand64_int64();
        sources[i] = (uint32_t)gen % num_vertices;
      } while (graph->row_ptr[sources[i] + 1] - graph->row_ptr[sources[i]] ==
               0);
    }
  }
  return sources;
}

int main(int argc, char **argv) {
  Cli_Args args;
  init_cli();
  if (parse_args(argc, argv, &args) != 0) {
    return -1;
  }

  graph = import_mtx(args.filename);
  if (graph == NULL) {
    fprintf(stderr, "Failed to import graph from file [%s]\n", args.filename);
    return -1;
  }
  uint32_t *sources =
      generate_sources(args.runs, graph->num_vertices, args.source);
  // print_sources(graph, sources, args.runs);
  printf("Threads: %d, Chunk size: %d\n", MAX_THREADS, CHUNK_SIZE);

  distances = malloc(graph->num_vertices * sizeof(uint32_t));
  memset(distances, UINT32_MAX, graph->num_vertices * sizeof(uint32_t));
  initialize_bfs();
  // print_graph(graph);

  struct timespec start, end;
  double elapsed[args.runs];
  for (int i = 0; i < args.runs; i++) {
    clock_gettime(CLOCK_MONOTONIC, &start);
    bfs(sources[i]);
    clock_gettime(CLOCK_MONOTONIC, &end);
    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    elapsed[i] = seconds + nanoseconds * 1e-9;
    printf("Trial time: %16.5f\n", elapsed[i]);
    // print_distances(distances);
    memset(distances, UINT32_MAX, graph->num_vertices * sizeof(uint32_t));
  }

  // Terminate threads
  notify_threads(&tp, 1);
  join_threads(&tp);

  print_time(elapsed, args.runs);

  free(sources);
  free_frontier(f1);
  free_frontier(f2);
  pthread_barrier_destroy(&barrier);
  free(graph->row_ptr);
  free(graph->col_idx);
  free(graph);
  free(distances);

  return 0;
}