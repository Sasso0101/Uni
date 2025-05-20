#include "bitmap.h"
#include "config.h"
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BITS_PER_WORD 64
#define WORD_INDEX(i) ((i) / BITS_PER_WORD)
#define BIT_OFFSET(i) ((i) % BITS_PER_WORD)

Bitmap *bitmap_create(uint32_t num_vertices) {
  Bitmap *b = malloc(sizeof(Bitmap));
  b->bitmap = calloc((num_vertices + BITS_PER_WORD - 1) / BITS_PER_WORD,
                     sizeof(uint64_t));
  b->size = num_vertices;
  return b;
}

void bitmap_destroy(Bitmap *b) {
  free(b->bitmap);
  free(b);
}

void bitmap_clear(Bitmap *b, uint32_t num_vertices) {
  memset(b->bitmap, 0,
         (num_vertices + BITS_PER_WORD - 1) / BITS_PER_WORD * sizeof(uint64_t));
}

void bitmap_set_bit(Bitmap *b, size_t index) {
  b->bitmap[WORD_INDEX(index)] |= (1ULL << BIT_OFFSET(index));
}

void bitmap_clear_bit(Bitmap *b, size_t index) {
  b->bitmap[WORD_INDEX(index)] &= ~(1ULL << BIT_OFFSET(index));
}

int bitmap_test_bit(const Bitmap *b, size_t index) {
  return (b->bitmap[WORD_INDEX(index)] >> BIT_OFFSET(index)) & 1;
}

void bitmap_calculate_offset(Bitmap* b, int thread_id, uint32_t *start, uint32_t *end) {
  // Calculate the total number of 64-bit words required for the bitmap.
  uint32_t total_bitmap_words = (b->size + BITS_PER_WORD - 1) / BITS_PER_WORD;

  uint32_t word_start_for_thread =
      (thread_id * total_bitmap_words) / MAX_THREADS;
  uint32_t word_end_for_thread =
      ((thread_id + 1) * total_bitmap_words) / MAX_THREADS;

  // The last thread might need to pick up remaining words if total_bitmap_words
  // is not evenly divisible by MAX_THREADS. This also ensures it doesn't go
  // past the end.
  if (thread_id == MAX_THREADS - 1) {
    word_end_for_thread = total_bitmap_words;
  }

  // Convert the word range to a vertex range.
  *start = word_start_for_thread * 64;
  *end = word_end_for_thread * 64;

  // Cap the end of the vertex processing range at the actual number
  // of vertices in the graph.
  if (*end > b->size) {
    *end = b->size;
  }
}