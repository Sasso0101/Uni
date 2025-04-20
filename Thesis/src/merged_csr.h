#ifndef MERGEDCSR_H
#define MERGEDCSR_H

#include "config.h"
#include "graph.h"

typedef struct {
  uint32_t num_vertices;
  uint32_t num_edges;
  mer_t *row_ptr;
  mer_t *merged;
} MergedCSR;

#define METADATA_SIZE 2
#define DEGREE(mer, i) mer->merged[i]
#define DISTANCE(mer, i) mer->merged[i + 1]

/**
 * Converts the CSR graph into a modified "merged CSR" format with embedded
 * metadata.
 *
 * Original CSR:
 *   - row_ptr[i] points to the start of the neighbors of vertex i in col_idx[]
 *   - col_idx[] contains the destination vertex IDs of edges
 *
 * Merged CSR format:
 *   - For each vertex i, we allocate METADATA_SIZE (2) slots before its
 * adjacency list.
 *   - These metadata slots are:
 *       DEGREE(i)   = number of neighbors (out-degree of vertex i)
 *       DISTANCE(i) = BFS distance, initialized to UINT32_MAX
 *   - The new col_idx[] is reallocated to hold both metadata and edges for all
 * vertices.
 *   - For each neighbor j of vertex i, we store the *offset* into col_idx[] for
 * j’s metadata (i.e., row_ptr[j] + j * METADATA_SIZE), not the raw vertex ID.
 *
 * The row_ptr[] is also updated so that row_ptr[i] points to the DEGREE slot
 * (i.e., the beginning of vertex i’s metadata).
 *
 * This layout allows efficient BFS traversal where each vertex's distance and
 * neighbor information are stored contiguously, improving cache performance.
 */
MergedCSR *to_merged_csr(const GraphCSR *graph) {
  MergedCSR *merged_csr = (MergedCSR *)malloc(sizeof(MergedCSR));
  merged_csr->num_edges = graph->num_edges;
  merged_csr->num_vertices = graph->num_vertices;
  merged_csr->row_ptr = (mer_t *)malloc((graph->num_vertices + 1) * sizeof(mer_t));
  merged_csr->merged =
      (mer_t *)malloc((graph->num_edges) * METADATA_SIZE * sizeof(mer_t));
  for (mer_t i = 0; i < graph->num_vertices; i++) {
    mer_t start = graph->row_ptr[i] + i * METADATA_SIZE;
    DEGREE(merged_csr, start) = graph->row_ptr[i + 1] - graph->row_ptr[i];
    DISTANCE(merged_csr, start) = UINT32_MAX;
    start += METADATA_SIZE;
    for (mer_t j = graph->row_ptr[i]; j < graph->row_ptr[i + 1]; j++) {
      merged_csr->merged[start++] =
          graph->row_ptr[graph->col_idx[j]] + graph->col_idx[j] * METADATA_SIZE;
    }
  }
  // Create new row_ptr accounting for the shift added by the metadata in the merged CSR
  for (mer_t i = 0; i < graph->num_vertices + 1; i++) {
    merged_csr->row_ptr[i] = graph->row_ptr[i] + i * METADATA_SIZE;
  }
  return merged_csr;
}

void destroy_merged_csr(MergedCSR *merged_csr) {
  free(merged_csr->merged);
  free(merged_csr->row_ptr);
  free(merged_csr);
}

#endif // MERGEDCSR_H