#include "bitmap.h"
#include "config.h"
#include "frontier.h"
#include "merged_csr.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define BITS_PER_WORD 64
#define WORD_INDEX(i) ((i) / BITS_PER_WORD)
#define BIT_OFFSET(i) ((i) % BITS_PER_WORD)

Bitmap *bitmap_create(uint32_t num_vertices) {
  Bitmap *b = malloc(sizeof(Bitmap));
  b->bitmap = calloc((num_vertices + BITS_PER_WORD - 1)/BITS_PER_WORD, sizeof(uint64_t));
  return b;
}

void bitmap_destroy(Bitmap *b) {
  free(b->bitmap);
  free(b);
}

void bitmap_clear(Bitmap *b, uint32_t num_vertices) {
  memset(b->bitmap, 0, (num_vertices + BITS_PER_WORD - 1)/BITS_PER_WORD * sizeof(uint64_t));
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