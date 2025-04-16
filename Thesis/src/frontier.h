#ifndef FRONTIER_H
#define FRONTIER_H

#include <pthread.h>
#include "config.h"
#include <stdint.h>

typedef struct {
  int vertices[CHUNK_SIZE];
  int next_free_index;
} VertexChunk;

typedef struct {
  VertexChunk** chunks;
  int chunks_size;
  int next_free_chunk;
  int next_uninitialized_chunk;
  pthread_mutex_t lock;
} ThreadVertexChunks;

typedef struct {
  ThreadVertexChunks** thread_chunks;
  int* chunk_counts;
} Frontier;

Frontier* init_frontier();
VertexChunk* create_chunk(Frontier *f, int thread_id);
VertexChunk* remove_chunk(Frontier *f, int thread_id);
uint32_t remove_vertex(VertexChunk* c);
void insert_vertex(VertexChunk* c, uint32_t v);
int get_max_thread(Frontier *f);
void free_frontier(Frontier* f);
int get_total_chunks(Frontier *f);

#endif // FRONTIER_H