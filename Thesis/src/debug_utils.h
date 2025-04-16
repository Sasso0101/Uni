#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include "config.h"
#include "frontier.h"
#include "merged_csr.h"
#include <stdint.h>

void print_vertex(GraphCSR *graph, uint32_t v) {
  printf("Vertex %d, degree %d, distance %d: ", v, DEGREE(graph, v), DISTANCE(graph, v));
  for (uint32_t i = v + METADATA_SIZE; i < v + METADATA_SIZE + DEGREE(graph, v); i++) {
    printf("%d ", graph->col_idx[i]);
  }
  printf("\n");
}

void print_chunk_counts(Frontier *f) {
  printf("Chunk counts: ");
  for (int i = 0; i < MAX_THREADS; i++) {
    printf("%5d", f->chunk_counts[i]);
  }
  printf("\n");
}

void print_sources(GraphCSR *graph, uint32_t *sources, int runs) {
  printf("Sources: ");
  for (int i = 0; i < runs; i++) {
    printf("%d (%d)\n", sources[i], graph->row_ptr[sources[i]]);
  }
  printf("\n");
}

void print_distances(GraphCSR *graph, uint32_t *distances) {
  printf("Distances:\n");
  for (uint32_t i = 0; i < graph->num_vertices; i++) {
    printf("Vertex %d: %d\n", i, distances[i]);
  }
}

#endif // DEBUG_UTILS_H