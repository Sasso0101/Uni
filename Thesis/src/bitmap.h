#ifndef BITMAP_H
#define BITMAP_H

#include "frontier.h"
#include "merged_csr.h"
#include <stdint.h>

typedef struct {
  uint64_t* bitmap;
} Bitmap;

Bitmap* bitmap_create(uint32_t num_vertices);
void bitmap_destroy(Bitmap *b);
void bitmap_clear(Bitmap *b, uint32_t num_vertices);
void bitmap_set_bit(Bitmap *b, size_t index);
void bitmap_clear_bit(Bitmap *b, size_t index);
int bitmap_test_bit(const Bitmap *b, size_t index);

#endif