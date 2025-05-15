#define _GNU_SOURCE
#include "bitmap.h"
#include "command_line.h"
#include "debug_utils.h"
#include "graph.h"
#include "merged_csr.h"
#include "thread_pool.h"
#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

MergedCSR *merged_csr;
GraphCSR *graph;
thread_pool_t tp;

atomic_uint_fast64_t total;

volatile uint32_t exploration_done;

void *thread_main(int thread_id) {
  while (!exploration_done) {
    int old = distance;
    mer_t edges_to_check = 0;
    uint32_t vert_in_front = 0;
    if (is_top_down) {
      top_down(f1, f2, distance, thread_id, &edges_to_check);
    } else {
      bottom_up(b1, b2, distance, thread_id, &vert_in_front, &edges_to_check);
      vert_in_front_tot += vert_in_front;
    }
    edges_to_check_front += edges_to_check;
    if (atomic_fetch_sub(&active_threads, 1) == 1) {
      // Swap frontiers
      active_threads = MAX_THREADS;
      if (is_top_down) {
        if (frontier_get_total_chunks(f2) == 0) {
          exploration_done = 1;
        } else if (edges_to_check_front > edges_to_check_tot / ALPHA) {
          is_top_down = 0;
          bitmap_clear(b1, merged_csr->num_vertices);
          struct timespec start, end;
          clock_gettime(CLOCK_MONOTONIC, &start);
          frontier_to_bitmap(b1, f2, merged_csr, thread_id); //TODO: make it parallel
          clock_gettime(CLOCK_MONOTONIC, &end);
          long seconds = end.tv_sec - start.tv_sec;
          long nanoseconds = end.tv_nsec - start.tv_nsec;
          double el = seconds + nanoseconds * 1e-9;
          printf("Front to bit time: %16.5f\n", el);
        }
        Frontier *temp = f2;
        f2 = f1;
        f1 = temp;
      } else {
        if (vert_in_front_tot == 0) {
          exploration_done = 1;
        } else if (vert_in_front_tot < graph->num_vertices / BETA) {
          is_top_down = 1;
          struct timespec start, end;
          clock_gettime(CLOCK_MONOTONIC, &start);
          bitmap_to_frontier(b2, f1, merged_csr, thread_id); //TODO: make it parallel
          clock_gettime(CLOCK_MONOTONIC, &end);
          long seconds = end.tv_sec - start.tv_sec;
          long nanoseconds = end.tv_nsec - start.tv_nsec;
          double el = seconds + nanoseconds * 1e-9;
          printf("bit to front time: %16.5f\n", el);
        }
        Bitmap *temp = b2;
        b2 = b1;
        b1 = temp;
        bitmap_clear(b2, graph->num_vertices);
      }
      edges_to_check_tot -= edges_to_check_front;
      edges_to_check_front = 0;
      vert_in_front_tot = 0;

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

void tc() {
  // Convert source vertex to mergedCSR index
  exploration_done = 0;
  active_threads = MAX_THREADS;
  total = 0;
  atomic_thread_fence(memory_order_seq_cst);
  thread_pool_start_wait(&tp);
}

void init_cli() {
  add_help_line('f', "file", "load graph from file", NULL);
  add_help_line('n', "runs", "number of runs", "1");
  add_help_line('c', "", "Checks TC correctness", NULL);
  add_help_line('h', "", "print this help message", NULL);
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