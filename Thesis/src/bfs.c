#define _GNU_SOURCE
#include "command_line.h"
#include "config.h"
#include "debug_utils.h"
#include "frontier.h"
#include "graph.h"
#include "bitmap.h"
#include "merged_csr.h"
#include "mt19937-64.h"
#include "thread_pool.h"
#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define ALPHA 4
#define BETA 24

MergedCSR *merged_csr;
GraphCSR *graph;
Frontier *f1, *f2;
Bitmap *b1, *b2;
uint32_t *distances;

atomic_int active_threads;
atomic_uint_fast64_t edges_to_check_front;
uint64_t edges_to_check_tot;
atomic_uint_fast32_t vert_in_front_tot;
bool is_top_down;

volatile uint32_t exploration_done;
volatile int distance;

void top_down_chunk(MergedCSR *merged_csr, Frontier *next, Chunk *c,
                    Chunk **dest, int distance, int thread_id,
                    mer_t *edges_to_check) {
  assert(c != NULL && "Chunk passed to top_down_chunk is NULL!");
  mer_t v = MERGED_MAX;
  while ((v = chunk_pop_vertex(c)) != MERGED_MAX) {
    mer_t end = v + DEGREE(merged_csr, v) + METADATA_SIZE;
    for (mer_t i = v + METADATA_SIZE; i < end; i++) {
      mer_t neighbor = merged_csr->merged[i];
      if (DISTANCE(merged_csr, neighbor) == UINT32_MAX) {
        DISTANCE(merged_csr, neighbor) = distance;
        if (DEGREE(merged_csr, neighbor) != 1) {
          if (*dest == NULL || (*dest)->next_free_index >= CHUNK_SIZE) {
            *dest = frontier_create_chunk(next, thread_id);
          }
          chunk_push_vertex(*dest, neighbor);
          *edges_to_check += DEGREE(merged_csr, neighbor);
        }
      }
    }
  }
}

void top_down(MergedCSR *merged_csr, Frontier *current_frontier,
              Frontier *next_frontier, int distance, int thread_id,
              mer_t *edges_to_check) {
  Chunk *c = NULL;
  Chunk *next_chunk = NULL;
  Chunk **dest = &next_chunk;
  // Run top-down step for all chunks belonging to the thread
  while ((c = frontier_remove_chunk(current_frontier, thread_id)) != NULL) {
    top_down_chunk(merged_csr, next_frontier, c, dest, distance, thread_id,
                   edges_to_check);
  }
  // Work stealing from other threads when finished processing chunks of this
  // thread
  bool work_to_do = true;
  while (work_to_do) {
    work_to_do = false;
    for (int i = 0; i < MAX_THREADS; i++) {
      if (current_frontier->chunk_counts[i] > 1) {
        work_to_do = 1;
        if ((c = frontier_remove_chunk(current_frontier, i)) != NULL) {
          top_down_chunk(merged_csr, next_frontier, c, dest, distance,
                         thread_id, edges_to_check);
        }
        i--;
      }
    }
  }
}

void bottom_up(MergedCSR *merged, GraphCSR *graph, Bitmap *current,
               Bitmap *next, int distance, int thread_id,
               uint32_t *vert_in_front, mer_t *edges_to_check) {
  uint32_t vert_per_thread = merged_csr->num_vertices / MAX_THREADS;
  uint32_t start = vert_per_thread * thread_id;
  uint32_t potential_end = vert_per_thread * (thread_id + 1);
  uint32_t end = (potential_end < merged_csr->num_vertices)
                     ? potential_end
                     : merged_csr->num_vertices;

  for (uint32_t v = start; v < end; v++) {
    for (uint32_t i = graph->row_ptr[v]; i < graph->row_ptr[v + 1]; i++) {
      uint32_t neighbor = graph->col_idx[i];
      if (current->bitmap[neighbor]) {
        DISTANCE(merged, merged->row_ptr[v]) = distance;
        next->bitmap[v] = 1;
        edges_to_check += DEGREE(merged, merged->row_ptr[v]);
        vert_in_front++;
      }
    }
  }
}

void finalize_distances(MergedCSR *merged_csr, int thread_id) {
  // Write distances from mergedCSR to distances array
  mer_t chunk_size = merged_csr->num_vertices / MAX_THREADS;
  mer_t start = thread_id * chunk_size;
  mer_t end = (thread_id == MAX_THREADS - 1) ? merged_csr->num_vertices
                                             : (thread_id + 1) * chunk_size;
  for (mer_t i = start; i < end; i++) {
    distances[i] = DISTANCE(merged_csr, merged_csr->row_ptr[i]);
    DISTANCE(merged_csr, merged_csr->row_ptr[i]) = UINT32_MAX;
  }
}

void *thread_main(void *arg) {
  int thread_id = *(int *)arg;

  while (!exploration_done) {
    int old = distance;
    mer_t edges_to_check = 0;
    uint32_t vert_in_front = 0;
    if (is_top_down) {
      top_down(merged_csr, f1, f2, distance, thread_id, &edges_to_check);
    } else {
      bottom_up(merged_csr, graph, b1, b2, distance, thread_id, &vert_in_front, &edges_to_check);
      vert_in_front_tot += vert_in_front;
    }
    edges_to_check_front += edges_to_check;
    if (atomic_fetch_sub(&active_threads, 1) == 1) {
      // Swap frontiers
      active_threads = MAX_THREADS;
      if (is_top_down) {
        if (frontier_get_total_chunks(f1) == 0)
          exploration_done = 1;
        else {
          if (edges_to_check_front > edges_to_check_tot / ALPHA) {
            printf("Bottom up %d", distance+1);
            is_top_down = 0;
            bitmap_clear(b1, merged_csr->num_vertices);
            frontier_to_bitmap(b1, f2, merged_csr, thread_id);
          }
          Frontier *temp = f2;
          f2 = f1;
          f1 = temp;
        }
      } else {
        if (vert_in_front == 0)
          exploration_done = 1;
        else {
          if (vert_in_front < graph->num_vertices / BETA) {
            printf("Top down %d", distance+1);
            is_top_down = 1;
            bitmap_to_frontier(b2, f1, merged_csr, thread_id);
          }
          Bitmap *temp = b2;
          b2 = b1;
          b1 = temp;
          bitmap_clear(b2, graph->num_vertices);
        }
      }
      edges_to_check_tot -= edges_to_check_front;
      Frontier *temp = f2;
      f2 = f1;
      f1 = temp;

      // printf("%u ", distance);
      // print_chunk_counts(f1);
      atomic_thread_fence(memory_order_seq_cst);
      distance++;
    }
    while (distance == old)
      ;
  }
  finalize_distances(merged_csr, thread_id);

  if (atomic_fetch_sub(&active_threads, 1) == 1) {
    // printf("Max distance: %u\n", distance);
    thread_pool_notify_parent(&tp);
  }
  return NULL;
}

void initialize_bfs() {
  merged_csr = to_merged_csr(graph);
  f1 = frontier_create();
  f2 = frontier_create();
  b1 = bitmap_create(graph->num_vertices);
  b2 = bitmap_create(graph->num_vertices);
  init_thread_pool(&tp, thread_main);
  thread_pool_create(&tp);
}

void bfs(uint32_t source) {
  // Convert source vertex to mergedCSR index
  source = merged_csr->row_ptr[source];
  DISTANCE(merged_csr, source) = 0;
  Chunk *c = frontier_create_chunk(f1, 0);
  chunk_push_vertex(c, source);
  exploration_done = 0;
  active_threads = MAX_THREADS;
  edges_to_check_front = 0;
  edges_to_check_tot = merged_csr->num_edges;
  distance = 1;
  is_top_down = 1;
  atomic_thread_fence(memory_order_seq_cst);
  thread_pool_start_wait(&tp);
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

  graph = import_mtx(args.filename, METADATA_SIZE, MERGED_MAX);
  if (graph == NULL) {
    printf("Failed to import graph from file [%s]\n", args.filename);
    return -1;
  }

  uint32_t *sources =
      generate_sources(args.runs, graph->num_vertices, args.source);
  // print_sources(graph, sources, args.runs);
  printf("Threads: %d, Chunk size: %d, Vertex size: %lu bytes\n", MAX_THREADS,
         CHUNK_SIZE, sizeof(mer_t));

  distances = malloc(graph->num_vertices * sizeof(uint32_t));
  memset(distances, UINT32_MAX, graph->num_vertices * sizeof(uint32_t));
  initialize_bfs();

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
    if (args.check) {
      check_bfs_correctness(graph, distances, sources[i]);
    }
    memset(distances, UINT32_MAX, merged_csr->num_vertices * sizeof(uint32_t));
  }
  // Terminate threads
  thread_pool_terminate(&tp);

  print_time(elapsed, args.runs);

  free(sources);
  free(graph->row_ptr);
  free(graph->col_idx);
  free(graph);
  frontier_destroy(f1);
  frontier_destroy(f2);
  bitmap_destroy(b1);
  bitmap_destroy(b2);
  destroy_thread_pool(&tp);
  destroy_merged_csr(merged_csr);
  free(distances);

  return 0;
}