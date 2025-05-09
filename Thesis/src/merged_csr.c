#include "merged_csr.h"

MergedCSR *to_merged_csr(const GraphCSR *graph) {
  MergedCSR *merged_csr = (MergedCSR *)malloc(sizeof(MergedCSR));

  merged_csr->num_edges = graph->num_edges;
  merged_csr->num_vertices = graph->num_vertices;
  merged_csr->row_ptr =
      (mer_t *)malloc((graph->num_vertices + 1) * sizeof(mer_t));
  merged_csr->merged = (mer_t *)malloc((graph->num_edges) * DATA_SIZE *
                                       METADATA_SIZE * sizeof(mer_t));

  for (mer_t i = 0; i < graph->num_vertices; i++) {
    mer_t start = graph->row_ptr[i] + i * METADATA_SIZE;
    uint32_t degree = graph->row_ptr[i + 1] - graph->row_ptr[i];
    DEGREE(merged_csr, start) = degree;
    DISTANCE(merged_csr, start) = UINT32_MAX;
    ID(merged_csr, start) = i;
    start += METADATA_SIZE;
    for (mer_t j = graph->row_ptr[i]; j < graph->row_ptr[i + 1]; j++) {
      merged_csr->merged[start] = graph->col_idx[j];
      merged_csr->merged[start + degree] =
          graph->row_ptr[graph->col_idx[j]] * DATA_SIZE +
          graph->col_idx[j] * METADATA_SIZE;
    }
  }
  // Create new row_ptr accounting for the shift added by the metadata in the
  // merged CSR
  for (mer_t i = 0; i < graph->num_vertices + 1; i++) {
    merged_csr->row_ptr[i] = graph->row_ptr[i] * DATA_SIZE + i * METADATA_SIZE;
  }
  return merged_csr;
}
