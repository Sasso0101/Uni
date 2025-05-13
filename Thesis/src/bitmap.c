#include "bitmap.h"
#include "config.h"
#include "frontier.h"
#include "merged_csr.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

Bitmap* bitmap_create(uint32_t num_vertices) {
  Bitmap *b = malloc(sizeof(Bitmap));
  b->bitmap = calloc(num_vertices, sizeof(bool));
  return b;
}

void bitmap_destroy(Bitmap *b) {
  free(b->bitmap);
  free(b);
}

void bitmap_clear(Bitmap *b, uint32_t num_vertices) {
  memset(b->bitmap, 0, sizeof(bool)*num_vertices);
}

void chunk_to_bitmap(Bitmap *b, VertexChunk *c, MergedCSR *merged) {
  mer_t v = MERGED_MAX;
  while ((v = chunk_pop_vertex(c)) != MERGED_MAX) {
    b->bitmap[ID(merged, v)] = 1;
  }
}

void frontier_to_bitmap(Bitmap *b, Frontier *f, MergedCSR *merged, int thread_id) {
  VertexChunk *c = NULL;
  VertexChunk *next_chunk = NULL;
  VertexChunk **dest = &next_chunk;
  // Run top-down step for all chunks belonging to the thread
  while ((c = frontier_release_chunk(f, thread_id)) != NULL) {
    chunk_to_bitmap(b, c, merged);
  }
  // Work stealing from other threads when finished processing chunks of this
  // thread
  bool work_to_do = true;
  while (work_to_do) {
    work_to_do = false;
    for (int i = 0; i < MAX_THREADS; i++) {
      if (f->chunk_counts[i] > 1) {
        work_to_do = 1;
        if ((c = frontier_release_chunk(f, i)) != NULL) {
          chunk_to_bitmap(b, c, merged);
        }
        i--;
      }
    }
  }
}

void bitmap_to_frontier(Bitmap *b, Frontier *f, MergedCSR *merged, int thread_id) {
  uint32_t vert_per_thread = merged->num_vertices/MAX_THREADS;
  uint32_t start = vert_per_thread*thread_id;
  uint32_t potential_end = vert_per_thread * (thread_id + 1);
  uint32_t end = (potential_end < merged->num_vertices) ? potential_end : merged->num_vertices;
  VertexChunk *dest = frontier_acquire_chunk(f, thread_id);
  for (uint32_t i = start; i < end; i++) {
    uint32_t v = b->bitmap[i];
    if (v) {
      if (dest == NULL || dest->next_free_index >= CHUNK_SIZE) {
        dest = frontier_acquire_chunk(f, thread_id);
      }
      chunk_push_vertex(dest, merged->row_ptr[v]);
    }
  }
}