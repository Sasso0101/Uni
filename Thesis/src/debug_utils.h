#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include "config.h"
#include "frontier.h"
#include "merged_csr.h"
#include <stdint.h>
#include <stdint.h>

void print_chunk_counts(const Frontier *f) {
  printf("Chunk counts: ");
  for (int i = 0; i < MAX_THREADS; i++) {
    printf("%5d", f->chunk_counts[i]);
  }
  printf("\n");
}

void print_sources(const GraphCSR *graph, const uint32_t *sources, int runs) {
  printf("Sources: ");
  for (int i = 0; i < runs; i++) {
    printf("%d (%d)\n", sources[i], graph->row_ptr[sources[i]]);
  }
  printf("\n");
}

void print_graph(const GraphCSR *graph) {
  for (uint32_t i = 0; i < graph->num_vertices; i++) {
    printf("Vertex %d: ", i);
    for (uint32_t j = graph->row_ptr[i]; j < graph->row_ptr[i + 1]; j++) {
      printf("%d ", graph->col_idx[j]);
    }
    printf("\n");
  }
}

void print_merged_csr(const MergedCSR *merged_csr) {
  for (uint32_t i = 0; i < merged_csr->num_vertices; i++) {
    printf("Vertex %d (%lu): ", i, merged_csr->row_ptr[i]);
    for (mer_t j = merged_csr->row_ptr[i]; j < merged_csr->row_ptr[i + 1]; j++) {
      printf("%lu ", merged_csr->merged[j]);
    }
    printf("\n");
  }
}

void print_distances(const GraphCSR *graph, const uint32_t *distances) {
  printf("Distances:\n");
  for (uint32_t i = 0; i < graph->num_vertices; i++) {
    printf("Vertex %d: %d\n", i, distances[i]);
  }
}

void print_time(const double elapsed[], int runs) {
  double total = 0;
  for (int i = 0; i < runs; i++) {
    total += elapsed[i];
  }
  printf("Average time: %14.5f\n", total / runs);
}

int check_bfs_correctness(const GraphCSR *graph, const uint32_t *distances, uint32_t source) {
  uint32_t n = graph->num_vertices;

  if (distances[source] != 0) {
    printf("Error: distance to source vertex %u is not 0 (got %u)\n", source, distances[source]);
    return 0;
  }

  for (uint32_t u = 0; u < n; u++) {
    uint32_t dist_u = distances[u];
    uint32_t start = graph->row_ptr[u];
    uint32_t end = graph->row_ptr[u + 1];

    if (end - start == 0 && distances[u] != UINT32_MAX) {
      printf("Error: vertex %d is not connected to the graph but has been assigned distance %d\n", u, distances[u]);
      return 0;
    }
    else if (distances[u] != UINT32_MAX) {
      for (uint32_t i = start; i < end; i++) {
        uint32_t v = graph->col_idx[i];
        uint32_t dist_v = distances[v];
  
        // Rule: if v is reachable from u, then their distances must differ by at most 1
        if (dist_v > dist_u + 1) {
          printf("Error: edge (%u -> %u) with dist[%u] = %u, dist[%u] = %u (too far apart)\n",
                  u, v, u, dist_u, v, dist_v);
          return 0;
        }
      }
    }
  }

  printf("BFS starting from vertex %d is valid.\n", source);
  return 1;
}

#endif // DEBUG_UTILS_H