#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include "frontier.h"
#include <stdio.h>


Frontier* init_frontier() {
  Frontier *f = (Frontier *)malloc(sizeof(Frontier));
  f->thread_chunks = (ThreadVertexChunks **)malloc(sizeof(ThreadVertexChunks *) * MAX_THREADS);
  f->chunk_counts = (int *)calloc(MAX_THREADS, sizeof(int));

  for (int i = 0; i < MAX_THREADS; i++) {
    f->thread_chunks[i] = (ThreadVertexChunks *)malloc(sizeof(ThreadVertexChunks));
    f->thread_chunks[i]->chunks = (VertexChunk **)malloc(sizeof(VertexChunk *) * CHUNKS_PER_THREAD);
    f->thread_chunks[i]->chunks_size = CHUNKS_PER_THREAD;
    f->thread_chunks[i]->next_free_chunk = 0;
    f->thread_chunks[i]->next_uninitialized_chunk = 0;
    f->chunk_counts[i] = 0;
    pthread_mutex_init(&f->thread_chunks[i]->lock, NULL);
  }
  return f;
}

void free_frontier(Frontier* f) {
  free(f->chunk_counts);
  for (int i = 0; i < MAX_THREADS; i++) {
    free(f->thread_chunks[i]->chunks);
    pthread_mutex_destroy(&f->thread_chunks[i]->lock);
  }
  free(f->thread_chunks);
  free(f);
}

VertexChunk* create_chunk(Frontier *f, int thread_id) {
  ThreadVertexChunks *thread = f->thread_chunks[thread_id];
  pthread_mutex_lock(&thread->lock);
  // Check if we need to resize the chunks array
  if (thread->next_free_chunk >= thread->chunks_size) {
    thread->chunks_size *= 2;
    thread->chunks = (VertexChunk **)realloc(thread->chunks, thread->chunks_size * sizeof(VertexChunk *));
  }
  // Check if we need to allocate a new chunk
  if (thread->next_free_chunk >= thread->next_uninitialized_chunk) {
    thread->chunks[thread->next_free_chunk] = (VertexChunk *)malloc(sizeof(VertexChunk));
    thread->next_uninitialized_chunk++;
  }
  VertexChunk *new_chunk = thread->chunks[thread->next_free_chunk];
  thread->next_free_chunk++;
  f->chunk_counts[thread_id]++;
  pthread_mutex_unlock(&thread->lock);
  return new_chunk;
}

VertexChunk* remove_chunk(Frontier *f, int thread_id) {
  ThreadVertexChunks *thread = f->thread_chunks[thread_id];
  pthread_mutex_lock(&thread->lock);
  if (thread->next_free_chunk > 0) {
    thread->next_free_chunk--;
    VertexChunk *chunk = thread->chunks[thread->next_free_chunk];
    f->chunk_counts[thread_id]--;
    pthread_mutex_unlock(&thread->lock);
    return chunk;
  } else {
    pthread_mutex_unlock(&thread->lock);
    return NULL; // No chunks available
  }
}

uint32_t remove_vertex(VertexChunk* c) {
  if (c->next_free_index > 0) {
    c->next_free_index--;
    return c->vertices[c->next_free_index];
  } else {
    return UINT32_MAX;
  }
}

void insert_vertex(VertexChunk* c, uint32_t v) {
  assert(c != NULL && "Trying to insert in NULL chunk!");
  assert(c->next_free_index < CHUNK_SIZE && "Trying to insert in full chunk!");
  c->vertices[c->next_free_index] = v;
  c->next_free_index++;
}

int get_max_thread(Frontier *f) {
  int max_thread = -1;
  int max_chunks = 1;
  for (int i = 0; i < MAX_THREADS; i++) {
    if (f->chunk_counts[i] > max_chunks) {
      max_thread = i;
    }
  }
  return max_thread;
}

int get_total_chunks(Frontier *f) {
  int total = 0;
  for (int i = 0; i < MAX_THREADS; i++) {
    total += f->chunk_counts[i];
  }
  return total;
}