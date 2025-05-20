#define _GNU_SOURCE
#include "bitmap.h"
#include "command_line.h"
#include "config.h"
#include "debug_utils.h"
#include "frontier.h"
#include "graph.h"
#include "merged_csr.h"
#include "mt19937-64.h"
#include "thread_pool.h"
#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h> // For bool type
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define ALPHA 15
#define BETA 18

MergedCSR *merged_csr; // Will be NULL if is_small_diameter_graph is true
GraphCSR *graph;
Frontier *f1, *f2;
Bitmap *b1, *b2;
uint32_t *distances;
thread_pool_t tp;

atomic_int active_threads;
atomic_uint_fast64_t edges_to_check_front;
uint64_t edges_to_check_tot;
atomic_uint_fast32_t vert_in_front_tot;
bool is_top_down; // Direction of BFS (TD or BU)
bool convert_data;
int distance; // Current BFS distance level being explored
uint32_t exploration_done;

volatile uint32_t iter;

bool is_small_diameter_graph;

void chunk_to_bitmap_csr(Bitmap *b, Chunk *c) {
  mer_t v = MERGED_MAX;
  while ((v = chunk_pop_vertex(c)) != MERGED_MAX) {
    bitmap_set_bit(b, v);
  }
}

void frontier_to_bitmap_csr(Bitmap *b, Frontier *f, int thread_id) {
  Chunk *c;
  while ((c = frontier_remove_chunk(f, thread_id)) != NULL) {
    chunk_to_bitmap_csr(b, c);
  }
  int i = (thread_id + 1) % MAX_THREADS;
  while (i != thread_id) {
    if (f->chunk_counts[i] > 0) {
      while ((c = frontier_remove_chunk(f, i)) != NULL) {
        chunk_to_bitmap_csr(b, c);
      }
    }
    i = (i + 1) % MAX_THREADS;
  }
}

void bitmap_to_frontier_csr(Bitmap *b, Frontier *f, int thread_id) {
  mer_t chunk_size = graph->num_vertices / MAX_THREADS;
  mer_t start = thread_id * chunk_size;
  mer_t end = (thread_id == MAX_THREADS - 1) ? graph->num_vertices
                                             : (thread_id + 1) * chunk_size;
  Chunk *dest = frontier_create_chunk(f, thread_id);
  for (uint32_t i = start; i < end; i++) {
    if (bitmap_test_bit(b, i)) {
      if (dest == NULL || dest->next_free_index >= CHUNK_SIZE) {
        dest = frontier_create_chunk(f, thread_id);
      }
      chunk_push_vertex(dest, i);
    }
  }
}

void top_down_chunk(Frontier *next, Chunk *c, Chunk **dest,
                    int current_distance, int thread_id,
                    uint64_t *edges_to_check) {
  assert(c != NULL && "Chunk passed to top_down_chunk is NULL!");
  mer_t v = MERGED_MAX;
  while ((v = chunk_pop_vertex(c)) != MERGED_MAX) {
    mer_t end_offset = v + DEGREE(merged_csr, v) + METADATA_SIZE;
    for (mer_t i = v + METADATA_SIZE; i < end_offset; i++) {
      mer_t neighbor = merged_csr->merged[i];
      if (DISTANCE(merged_csr, neighbor) == UINT32_MAX) {
        DISTANCE(merged_csr, neighbor) = current_distance;
        if (*dest == NULL || (*dest)->next_free_index >= CHUNK_SIZE) {
          *dest = frontier_create_chunk(next, thread_id);
        }
        chunk_push_vertex(*dest, neighbor);
        *edges_to_check += DEGREE(merged_csr, neighbor);
      }
    }
  }
}

void top_down(Frontier *current_frontier, Frontier *next_frontier,
              int current_distance, int thread_id, uint64_t *edges_to_check) {
  Chunk *c = NULL;
  Chunk *next_chunk = NULL;
  Chunk **dest = &next_chunk;
  while ((c = frontier_remove_chunk(current_frontier, thread_id)) != NULL) {
    top_down_chunk(next_frontier, c, dest, current_distance, thread_id,
                   edges_to_check);
  }
  int i = (thread_id + 1) % MAX_THREADS;
  while (i != thread_id) {
    if (current_frontier->chunk_counts[i] > 0) {
      while ((c = frontier_remove_chunk(current_frontier, i)) != NULL) {
        top_down_chunk(next_frontier, c, dest, current_distance, thread_id,
                       edges_to_check);
      }
    }
    i = (i + 1) % MAX_THREADS;
  }
}

void finalize_distances(MergedCSR *merged, int thread_id) {
  mer_t chunk_size = merged->num_vertices / MAX_THREADS;
  mer_t start = thread_id * chunk_size;
  mer_t end = (thread_id == MAX_THREADS - 1) ? merged->num_vertices
                                             : (thread_id + 1) * chunk_size;
  for (mer_t i = start; i < end; i++) {
    distances[i] = DISTANCE(merged, merged->row_ptr[i]);
    DISTANCE(merged, merged->row_ptr[i]) =
        UINT32_MAX; // Reset MergedCSR distance
  }
}

void top_down_chunk_csr(Frontier *next, Chunk *c, Chunk **dest,
                        int current_distance, int thread_id,
                        uint64_t *edges_in_front) {
  assert(c != NULL && "Chunk passed to top_down_chunk_csr is NULL!");
  mer_t v;
  while ((v = chunk_pop_vertex(c)) != MERGED_MAX) {
    for (uint64_t i = graph->row_ptr[v]; i < graph->row_ptr[v + 1]; i++) {
      uint32_t neighbor = graph->col_idx[i];
      if (distances[neighbor] == UINT32_MAX) {
        distances[neighbor] = current_distance;
        if (*dest == NULL || (*dest)->next_free_index >= CHUNK_SIZE) {
          *dest = frontier_create_chunk(next, thread_id);
        }
        chunk_push_vertex(*dest, neighbor);
        *edges_in_front +=
            (graph->row_ptr[neighbor + 1] - graph->row_ptr[neighbor]);
      }
    }
  }
}

void top_down_csr(Frontier *current_frontier, Frontier *next_frontier,
                  int current_distance, int thread_id,
                  uint64_t *edges_in_front) {
  Chunk *c = NULL;
  Chunk *next_chunk = NULL;
  Chunk **dest = &next_chunk;
  while ((c = frontier_remove_chunk(current_frontier, thread_id)) != NULL) {
    top_down_chunk_csr(next_frontier, c, dest, current_distance, thread_id,
                       edges_in_front);
  }
  int i = (thread_id + 1) % MAX_THREADS;
  while (i != thread_id) {
    if (current_frontier->chunk_counts[i] > 0) {
      while ((c = frontier_remove_chunk(current_frontier, i)) != NULL) {
        top_down_chunk_csr(next_frontier, c, dest, current_distance, thread_id,
                           edges_in_front);
      }
    }
    i = (i + 1) % MAX_THREADS;
  }
}

void bottom_up_csr(Bitmap *current, Bitmap *next, int current_distance,
                   int thread_id, uint32_t *vert_in_front) {
  uint32_t start, end;
  bitmap_calculate_offset(next, thread_id, &start, &end);

  for (uint32_t v = start; v < end; v++) {
    if (distances[v] == UINT32_MAX) {
      for (uint64_t i = graph->row_ptr[v]; i < graph->row_ptr[v + 1]; i++) {
        uint32_t neighbor = graph->col_idx[i];
        if (bitmap_test_bit(current,
                            neighbor)) { // If neighbor is in current frontier
          bitmap_set_bit(next, v);
          distances[v] = current_distance;
          (*vert_in_front)++;
          break;
        }
      }
    }
  }
}

// --- Main Thread Logic ---
void *thread_main(int thread_id) {
  while (!exploration_done) {
    /*struct timespec start_ts, end_ts;
    clock_gettime(CLOCK_MONOTONIC, &start_ts);*/

    uint32_t current_iter = iter;
    int current_distance = distance;
    uint64_t local_edges_in_front = 0;
    uint32_t local_vert_in_front = 0;

    if (is_small_diameter_graph) {
      if (convert_data) {
        if (is_top_down) {
          bitmap_to_frontier_csr(b1, f1, thread_id);
        } else {
          if (thread_id == 0) {
            frontier_to_bitmap_csr(b1, f1, thread_id);
          }
        }
      } else {
        if (is_top_down) {
          top_down_csr(f1, f2, current_distance, thread_id,
                       &local_edges_in_front);
        } else { // Bottom-up CSR
          bottom_up_csr(b1, b2, current_distance, thread_id,
                        &local_vert_in_front);
          atomic_fetch_add_explicit(&vert_in_front_tot, local_vert_in_front,
                                    memory_order_relaxed);
        }
      }
    } else { // Large Diameter: MergedCSR, Top-Down ONLY
      top_down(f1, f2, current_distance, thread_id, &local_edges_in_front);
    }
    atomic_fetch_add_explicit(&edges_to_check_front, local_edges_in_front,
                              memory_order_relaxed);

    if (atomic_fetch_sub(&active_threads, 1) == 1) {
      active_threads = MAX_THREADS;
      /*clock_gettime(CLOCK_MONOTONIC, &end_ts);
      long seconds = end_ts.tv_sec - start_ts.tv_sec;
      long nanoseconds = end_ts.tv_nsec - start_ts.tv_nsec;
      double elapsed = seconds + nanoseconds * 1e-9;*/

      if (is_small_diameter_graph) {
        if (is_top_down) {
          if (convert_data) {
            // printf("%5d: %8s %16.5f\n", distance, "c b->t", elapsed);
            convert_data = false;
          } else if (frontier_get_total_chunks(f2) == 0) {
            // printf("%5d: %8s %16.5f\n", distance, "tp done", elapsed);
            exploration_done = 1;
          } else {
            if (edges_to_check_front > edges_to_check_tot / ALPHA) {
              is_top_down = false;
              convert_data = true;
              bitmap_clear(b1, graph->num_vertices);
            }
            Frontier *temp_f = f1;
            f1 = f2;
            f2 = temp_f;
            // printf("%5d: %8s %16.5f\n", distance, "td", elapsed);
          }
        } else { // Bottom-Up CSR
          if (convert_data) {
            // printf("%5d: %8s %16.5f\n", distance, "c t>b", elapsed);
            convert_data = false;
          } else if (vert_in_front_tot == 0) {
            // printf("%5d: %8s %16.5f\n", distance, "bu done", elapsed);
            exploration_done = 1;
          } else {
            if (vert_in_front_tot < graph->num_vertices / BETA) {
              is_top_down = true;
              convert_data = true;
            }
            Bitmap *temp_b = b1;
            b1 = b2;
            b2 = temp_b;
            bitmap_clear(b2, graph->num_vertices);
            // printf("%5d: %8s %16.5f\n", distance, "bu", elapsed);
          }
        }
      } else {
        if (frontier_get_total_chunks(f2) == 0) {
          exploration_done = 1;
        }
        Frontier *temp_f = f1;
        f1 = f2;
        f2 = temp_f;
      }

      uint64_t current_edges_in_front = edges_to_check_front;
      if (edges_to_check_tot > current_edges_in_front) {
        edges_to_check_tot -= current_edges_in_front;
      } else {
        edges_to_check_tot = 0;
      }

      atomic_store_explicit(&edges_to_check_front, 0, memory_order_relaxed);
      atomic_store_explicit(&vert_in_front_tot, 0, memory_order_relaxed);

      // before distance increment
      if (!convert_data) {
        distance++;
      }
      atomic_thread_fence(memory_order_seq_cst); // Ensure all writes visible
      iter++;
    }
    // Spin-wait for the iteration number to be incremented by the last thread.
    while (current_iter == iter) {
// Could yield or sleep briefly for heavily contended scenarios, but spin is
// common.
#if defined(__GNUC__) || defined(__clang__)
      // __builtin_ia32_pause(); // x86 pause instruction, or similar for other
      // archs
#endif
    }
  }

  if (!is_small_diameter_graph) {
    finalize_distances(merged_csr, thread_id);
  }

  if (atomic_fetch_sub_explicit(&active_threads, 1, memory_order_acq_rel) ==
      1) {
    thread_pool_notify_parent(&tp);
  }
  return NULL;
}

void initialize_bfs() {
  if (!is_small_diameter_graph) {
    merged_csr = to_merged_csr(graph);
    if (!merged_csr) {
      fprintf(stderr, "Failed to create MergedCSR.\n");
      exit(EXIT_FAILURE);
    }
  } else {
    merged_csr = NULL; // Explicitly NULL for small diameter graphs
  }

  f1 = frontier_create();
  f2 = frontier_create();
  b1 = bitmap_create(graph->num_vertices);
  b2 = bitmap_create(graph->num_vertices);
  init_thread_pool(&tp, thread_main);
  thread_pool_create(&tp);
}

void bfs(uint32_t source_node_id) {
  // Clear data structures for a new BFS run
  bitmap_clear(b1, graph->num_vertices);
  bitmap_clear(b2, graph->num_vertices);

  // Reset global state for BFS
  active_threads = MAX_THREADS;
  edges_to_check_front = 0;
  vert_in_front_tot = 0;
  is_top_down = true;
  convert_data = false;
  distance = 1;
  exploration_done = 0;
  iter = 1;

  if (is_small_diameter_graph) {
    edges_to_check_tot = graph->num_edges;
    distances[source_node_id] = 0;
    Chunk *c = frontier_create_chunk(f1, 0);
    chunk_push_vertex(c, (mer_t)source_node_id);
  } else {
    // Large diameter: MergedCSR, top-down only
    edges_to_check_tot = merged_csr->num_edges;
    mer_t merged_source_ptr = merged_csr->row_ptr[source_node_id];
    DISTANCE(merged_csr, merged_source_ptr) =
        0; // Set source distance in MergedCSR
    Chunk *c = frontier_create_chunk(f1, 0);
    chunk_push_vertex(c, merged_source_ptr);
  }

  atomic_thread_fence(memory_order_seq_cst); // Ensure all initializations are
                                             // visible to worker threads
  thread_pool_start_wait(&tp);
}

uint32_t *generate_sources(int runs, uint32_t num_vertices,
                           const char *source) {
  uint32_t *sources_arr = malloc(runs * sizeof(uint32_t));
  if (!sources_arr) {
    perror("Failed to allocate sources array");
    exit(EXIT_FAILURE);
  }
  if (!str_is_empty(source)) {
    char *endptr;
    uint32_t vertex = strtoul(source, &endptr, 10);
    if (*endptr != '\0') {
      for (int i = 0; i < runs; i++) {
        sources_arr[i] = vertex;
      }
    } else {
      perror("Passed source is not a valid integer.");
      exit(EXIT_FAILURE);
    }
  } else {
    init_genrand64(SEED);
    for (int i = 0; i < runs; i++) {
      do {
        uint64_t gen = genrand64_int64();
        sources_arr[i] = (uint32_t)gen % num_vertices;
      } while (graph->row_ptr[sources_arr[i] + 1] -
                   graph->row_ptr[sources_arr[i]] ==
               0); // Ensure source has out-degree > 0
    }
  }
  return sources_arr;
}

typedef struct {
  char filename[MAX_STRING_ARG_LEN];
  char source[MAX_STRING_ARG_LEN];
  int num_runs;
  bool logging;
  bool verify;
} ApplicationSettings;

ApplicationSettings settings;

void init_options() {
  settings.filename[0] = '\0';
  settings.source[0] = '\0';
  add_option('f', "MatrixMarket file path", true, "filepath", "",
             ARG_TYPE_STRING, &settings.filename, true);
  add_option('n', "Number of runs", true, "count", "1", ARG_TYPE_INT,
             &settings.num_runs, false);
  add_option('l', "Enable detailed logging", false, NULL, NULL,
             ARG_TYPE_BOOL, &settings.logging, false);
  add_option('v', "Enable correctness check for each run", false, NULL, NULL,
             ARG_TYPE_BOOL, &settings.logging, false);
  add_option('s', "Source vertex", true, "ID", "", ARG_TYPE_STRING,
             &settings.source, false);
}

int main(int argc, char **argv) {
  cli_parser_init();
  init_options();

  int parse_status = parse_args(argc, argv);
  if (parse_status == HELP_PRINTED) {
    return 0;
  }
  if (parse_status != PARSE_SUCCESS) {
    fprintf(stderr, "Exiting due to argument parsing error.\n");
    return 1;
  }

  graph = import_mtx(settings.filename, METADATA_SIZE, MERGED_MAX);
  if (graph == NULL) {
    printf("Failed to import graph from file [%s]\n", settings.filename);
    return -1;
  }

  // Determine graph diameter characteristic
  if (graph->num_vertices > 0 &&
      graph->num_edges > 0) { // Avoid division by zero
    is_small_diameter_graph =
        ((double)graph->num_edges / graph->num_vertices > 10.0);
  } else {
    is_small_diameter_graph = false; // Default for empty or edgeless graphs
  }

  uint32_t *sources =
      generate_sources(settings.num_runs, graph->num_vertices, settings.source);
  printf("Threads: %d, Chunk size: %d, Vertex size: %lu bytes, Small diameter "
         "mode: %s\n",
         MAX_THREADS, CHUNK_SIZE, sizeof(mer_t),
         is_small_diameter_graph ? "Yes" : "No");

  distances = malloc(graph->num_vertices * sizeof(uint32_t));
  if (!distances) {
    perror("Failed to allocate distances array");
    return -1;
  }
  for (uint32_t i = 0; i < graph->num_vertices; i++) {
    distances[i] = UINT32_MAX;
  }

  initialize_bfs();

  struct timespec start_ts, end_ts;
  double elapsed[settings.num_runs];
  for (int i = 0; i < settings.num_runs; i++) {
    // Reset distances for each run
    for (uint32_t j = 0; j < graph->num_vertices; j++) {
      distances[j] = UINT32_MAX;
    }

    clock_gettime(CLOCK_MONOTONIC, &start_ts);
    bfs(sources[i]);
    clock_gettime(CLOCK_MONOTONIC, &end_ts);
    long seconds = end_ts.tv_sec - start_ts.tv_sec;
    long nanoseconds = end_ts.tv_nsec - start_ts.tv_nsec;
    elapsed[i] = seconds + nanoseconds * 1e-9;
    printf("Trial time: %16.5f\n", elapsed[i]);

    if (settings.verify) {
      check_bfs_correctness(graph, distances, sources[i]);
    }
  }

  thread_pool_terminate(&tp);

  print_time(elapsed, settings.num_runs);

  free(sources);
  free(graph->row_ptr);
  free(graph->col_idx);
  free(graph);
  frontier_destroy(f1);
  frontier_destroy(f2);
  bitmap_destroy(b1);
  bitmap_destroy(b2);
  destroy_thread_pool(&tp);
  if (merged_csr) { // Conditionally destroy merged_csr
    destroy_merged_csr(merged_csr);
  }
  free(distances);

  return 0;
}