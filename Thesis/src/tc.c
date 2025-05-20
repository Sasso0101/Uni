#define _GNU_SOURCE
#include "array_processor.h"
#include "command_line.h"
#include "debug_utils.h"
#include "graph.h"
#include "merged_csr.h"
#include "thread_pool.h"
#include <stdatomic.h>
#include <time.h>

#define TC_CHUNK_SIZE 1024

MergedCSR *merged_csr;
GraphCSR *graph;
ArrayProcessor ap;
thread_pool_t tp;

atomic_int active_threads;
atomic_uint_fast32_t total;

// --- Main Thread Logic ---
void *thread_main(__attribute__((unused)) int thread_id) {
  size_t start_index;
  size_t elements_in_chunk;
  uint32_t local_total = 0;
  while (array_processor_get_chunk(&ap, &start_index, &elements_in_chunk)) {
    // printf("[%d] processing chunk from index %zu\n", thread_id, start_index);
    uint32_t u, v, w;
    for (uint32_t i = start_index; i < start_index + elements_in_chunk; i++) {
      u = merged_csr->row_ptr[i];
      for (uint32_t j = u + METADATA_SIZE; j < u + METADATA_SIZE + DEGREE(merged_csr, u); j++) {
        v = merged_csr->merged[j];
        if (v > u)
          break;
        uint32_t it = v + METADATA_SIZE;
        for (uint32_t t = u + METADATA_SIZE; t < u + METADATA_SIZE + DEGREE(merged_csr, u); t++) {
          w = merged_csr->merged[t];
          if (w > v)
            break;
          while (merged_csr->merged[it] < w)
            it++;
          if (merged_csr->merged[it] == w)
            local_total++;
        }
      }
    }
  }
  total += local_total;
  if (atomic_fetch_sub_explicit(&active_threads, 1, memory_order_acq_rel) ==
      1) {
    thread_pool_notify_parent(&tp);
  }
  // print_current_time_ms(thread_id);
  return NULL;
}

void initialize_tc() {
  merged_csr = to_merged_csr(graph);
  if (!merged_csr) {
    fprintf(stderr, "Failed to create MergedCSR.\n");
    exit(EXIT_FAILURE);
  }
  init_thread_pool(&tp, thread_main);
  thread_pool_create(&tp);
}

void tc() {
  active_threads = MAX_THREADS;
  total = 0;
  if (array_processor_init(&ap, merged_csr->merged, merged_csr->num_vertices,
                           TC_CHUNK_SIZE) != 0) {
    fprintf(stderr, "Failed to initialize array processor.\n");
    exit(EXIT_FAILURE);
  }
  atomic_thread_fence(memory_order_seq_cst); // Ensure all initializations are
                                             // visible to worker threads
  thread_pool_start_wait(&tp);
}

typedef struct {
  char filename[MAX_STRING_ARG_LEN];
  int num_runs;
  bool logging;
} ApplicationSettings;

ApplicationSettings settings;

void init_options() {
  settings.filename[0] = '\0';
  add_option('f', "MatrixMarket file path", true, "filepath", "",
             ARG_TYPE_STRING, &settings.filename, true);
  add_option('n', "Number of runs", true, "count", "1", ARG_TYPE_INT,
             &settings.num_runs, false);
  add_option('l', "Enable detailed logging", false, NULL, NULL, ARG_TYPE_BOOL,
             &settings.logging, false);
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

  initialize_tc();

  struct timespec start_ts, end_ts;
  double elapsed[settings.num_runs];
  for (int i = 0; i < settings.num_runs; i++) {

    clock_gettime(CLOCK_MONOTONIC, &start_ts);
    tc();
    clock_gettime(CLOCK_MONOTONIC, &end_ts);
    long seconds = end_ts.tv_sec - start_ts.tv_sec;
    long nanoseconds = end_ts.tv_nsec - start_ts.tv_nsec;
    elapsed[i] = seconds + nanoseconds * 1e-9;
    printf("Trial time: %16.5f Triangles: %lu\n", elapsed[i], total);
  }

  thread_pool_terminate(&tp);

  print_time(elapsed, settings.num_runs);

  free(graph->row_ptr);
  free(graph->col_idx);
  free(graph);
  destroy_thread_pool(&tp);
  destroy_merged_csr(merged_csr);

  return 0;
}