#ifndef MERGEDCSR_H
#define MERGEDCSR_H

#include "graph.h"

#define METADATA_SIZE 2
#define DEGREE(g, i) g->col_idx[i]
#define DISTANCE(g, i) g->col_idx[i + 1]

/**
 * Converts the CSR graph into a modified "merged CSR" format with embedded metadata.
 *
 * Original CSR:
 *   - row_ptr[i] points to the start of the neighbors of vertex i in col_idx[]
 *   - col_idx[] contains the destination vertex IDs of edges
 *
 * Merged CSR format:
 *   - For each vertex i, we allocate METADATA_SIZE (2) slots before its adjacency list.
 *   - These metadata slots are:
 *       DEGREE(i)   = number of neighbors (out-degree of vertex i)
 *       DISTANCE(i) = BFS distance, initialized to UINT32_MAX
 *   - The new col_idx[] is reallocated to hold both metadata and edges for all vertices.
 *   - For each neighbor j of vertex i, we store the *offset* into col_idx[] for j’s metadata
 *     (i.e., row_ptr[j] + j * METADATA_SIZE), not the raw vertex ID.
 * 
 * The row_ptr[] is also updated so that row_ptr[i] points to the DEGREE slot
 * (i.e., the beginning of vertex i’s metadata).
 *
 * This layout allows efficient BFS traversal where each vertex's distance and neighbor
 * information are stored contiguously, improving cache performance.
 */
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