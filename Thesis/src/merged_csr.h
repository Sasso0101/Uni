#ifndef MERGEDCSR_H
#define MERGEDCSR_H

#include "config.h"
#include "graph.h"
#include <stdlib.h>

typedef struct {
  uint32_t num_vertices;
  uint32_t num_edges;
  mer_t *row_ptr;
  mer_t *merged;
} MergedCSR;

#define METADATA_SIZE 3
#define DEGREE(mer, i) mer->merged[i]
#define DISTANCE(mer, i) mer->merged[i + 1]
#define ID(mer, i) mer->merged[i + 2]

/**
 * Converts the CSR graph into a modified "merged CSR" format with embedded
 * metadata.
 *
 * Original CSR:
 *   - row_ptr[i] points to the start of the neighbors of vertex i in col_idx[]
 *   - col_idx[] contains the vertex IDs of neighbors
 *
 * Merged CSR format:
 *   - For each vertex i, we allocate METADATA_SIZE slots before its
 * adjacency list.
 *   - These metadata slots are:
 *       DEGREE(i)   = number of neighbors (out-degree of vertex i)
 *       DISTANCE(i) = BFS distance, initialized to UINT32_MAX
 *       ID(i)       = Original vertex ID
 *   - The new col_idx[] is reallocated to hold both metadata and edges for all
 * vertices.
 *   - For each neighbor j of vertex i, we store the *offset* into col_idx[] for
 * j’s metadata (i.e., row_ptr[j] + j * METADATA_SIZE).
 *
 * The row_ptr[] is also updated so that row_ptr[i] points to the DEGREE slot
 * (i.e., the beginning of vertex i’s metadata).
 *
 * This layout improves cache performance by having vertex's distance and
 * neighbor information stored contiguously and not needing a lookup in row_ptr
 * while exploring neighbors and where each vertex's distance and neighbor
 * information are stored contiguously.
 */

MergedCSR *to_merged_csr(const GraphCSR *graph);

void destroy_merged_csr(MergedCSR *merged_csr);

#endif // MERGEDCSR_H