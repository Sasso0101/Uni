#ifndef BITMAP_H
#define BITMAP_H

#include "frontier.h"
#include "merged_csr.h"
#include <stdint.h>

typedef struct {
  uint32_t* bitmap;
} Bitmap;

Bitmap* bitmap_create(uint32_t num_vertices);
void bitmap_destroy(Bitmap *b);
void bitmap_clear(Bitmap *b, uint32_t num_vertices);
void frontier_to_bitmap(Bitmap *b, Frontier *f, MergedCSR *merged, int thread_id);
void bitmap_to_frontier(Bitmap *b, Frontier *f, MergedCSR *merged, int thread_id);

#endif