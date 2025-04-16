#ifndef MERGEDCSR_H
#define MERGEDCSR_H

#include "graph.h"

#define METADATA_SIZE 2
#define DEGREE(g, i) g->col_idx[i]
#define DISTANCE(g, i) g->col_idx[i + 1]

void to_merged_csr(GraphCSR *graph) {
  uint32_t *merged_csr =
      (uint32_t *)malloc((graph->num_edges) * METADATA_SIZE * sizeof(uint32_t));
  uint32_t *col_idx = graph->col_idx;
  graph->col_idx = merged_csr;
  for (uint32_t i = 0; i < graph->num_vertices; i++) {
    uint32_t start = graph->row_ptr[i] + i * METADATA_SIZE;
    DEGREE(graph, start) = graph->row_ptr[i + 1] - graph->row_ptr[i];
    DISTANCE(graph, start) = UINT32_MAX;
    start += METADATA_SIZE;
    for (uint32_t j = graph->row_ptr[i]; j < graph->row_ptr[i + 1]; j++) {
      merged_csr[start++] =
          graph->row_ptr[col_idx[j]] + col_idx[j] * METADATA_SIZE;
    }
  }
  for (uint32_t i = 0; i < graph->num_vertices + 1; i++) {
    graph->row_ptr[i] = graph->row_ptr[i] + i * METADATA_SIZE;
  }
  free(col_idx);
}

#endif // MERGEDCSR_H