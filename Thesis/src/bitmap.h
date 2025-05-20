#ifndef BITMAP_H
#define BITMAP_H

#include "frontier.h"
#include "merged_csr.h"
#include <stdint.h>

typedef struct {
  uint64_t *bitmap;
  uint32_t size;
} Bitmap;

Bitmap *bitmap_create(uint32_t num_vertices);
void bitmap_destroy(Bitmap *b);
void bitmap_clear(Bitmap *b, uint32_t num_vertices);
void bitmap_set_bit(Bitmap *b, size_t index);
void bitmap_clear_bit(Bitmap *b, size_t index);
int bitmap_test_bit(const Bitmap *b, size_t index);

/*
 * Determine the range of bitmap words this thread will be responsible for. This
 * partitioning ensures that different threads will write to disjoint uint64_t
 * words in the 'next' bitmap, preventing race conditions on bitmap_set_bit.
 */
void bitmap_calculate_offset(Bitmap *b, int thread_id, uint32_t *start,
                             uint32_t *end);

#endif