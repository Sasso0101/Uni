#include "frontier.h"
#include "config.h"
#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Frontier *frontier_create() {
  Frontier *f = (Frontier *)malloc(sizeof(Frontier));
  f->thread_chunks =
      (ChunkPool **)malloc(sizeof(ChunkPool *) * MAX_THREADS);
  f->chunk_counts = (int *)calloc(MAX_THREADS, sizeof(int));

  for (int i = 0; i < MAX_THREADS; i++) {
    f->thread_chunks[i] =
        (ChunkPool *)malloc(sizeof(ChunkPool));
    f->thread_chunks[i]->chunks =
        (VertexChunk **)malloc(sizeof(VertexChunk *) * CHUNKS_PER_THREAD);
    f->thread_chunks[i]->chunks_size = CHUNKS_PER_THREAD;
    f->thread_chunks[i]->next_free_chunk = 0;
    f->thread_chunks[i]->initialized_count = 0;
    f->chunk_counts[i] = 0;
    pthread_mutex_init(&f->thread_chunks[i]->lock, NULL);
  }
  return f;
}


void destroy_frontier(Frontier *f) {
  for (int i = 0; i < MAX_THREADS; i++) {
    for (int j = 0; j < f->thread_chunks[i]->initialized_count; j++) {
      free(f->thread_chunks[i]->chunks[j]);
    }
    free(f->thread_chunks[i]->chunks);
    pthread_mutex_destroy(&f->thread_chunks[i]->lock);
  }
  free(f->thread_chunks);
  free(f->chunk_counts);
  free(f);
}

VertexChunk *frontier_acquire_chunk(Frontier *f, int thread_id) {
  ChunkPool *thread = f->thread_chunks[thread_id];
  pthread_mutex_lock(&thread->lock);
  // Check if we need to resize the chunks array
  if (thread->next_free_chunk >= thread->chunks_size) {
    thread->chunks_size *= 2;
    thread->chunks = (VertexChunk **)realloc(
        thread->chunks, thread->chunks_size * sizeof(VertexChunk *));
  }
  // Check if we need to allocate a new chunk
  if (thread->next_free_chunk >= thread->initialized_count) {
    thread->chunks[thread->next_free_chunk] =
        (VertexChunk *)malloc(sizeof(VertexChunk));
    thread->chunks[thread->next_free_chunk]->next_free_index = 0;
    thread->initialized_count++;
  }
  f->chunk_counts[thread_id]++;
  pthread_mutex_unlock(&thread->lock);
  return thread->chunks[thread->next_free_chunk++];
}


VertexChunk *frontier_release_chunk(Frontier *f, int thread_id) {
  ChunkPool *thread = f->thread_chunks[thread_id];
  pthread_mutex_lock(&thread->lock);
  if (thread->next_free_chunk > 0) {
    thread->next_free_chunk--;
    VertexChunk *chunk = thread->chunks[thread->next_free_chunk];
    f->chunk_counts[thread_id]--;
    pthread_mutex_unlock(&thread->lock);
    return chunk;
  } else {
    pthread_mutex_unlock(&thread->lock);
    return NULL; // No allocated chunks to be released
  }
}

void chunk_push_vertex(VertexChunk *c, mer_t v) {
  assert(c != NULL && "Trying to insert in NULL chunk!");
  assert(c->next_free_index < CHUNK_SIZE && "Trying to insert in full chunk!");
  c->vertices[c->next_free_index] = v;
  c->next_free_index++;
}

mer_t chunk_pop_vertex(VertexChunk *c) {
  if (c->next_free_index > 0) {
    c->next_free_index--;
    return c->vertices[c->next_free_index];
  } else {
    return MERGED_MAX;
  }
}

int frontier_get_stealable_thread(Frontier *f) {
  int max_thread = -1;
  // Stop stealing if threads have at most one chunk
  // Done because of degraded performance when a thread
  // starts an iteration without any vertices to process
  // and steals them from another thread that hasn't started
  // executing yet, breaking cache locality
  int max_chunks = 1;
  for (int i = 0; i < MAX_THREADS; i++) {
    if (f->chunk_counts[i] > max_chunks) {
      max_thread = i;
    }
  }
  return max_thread;
}

int frontier_get_total_chunks(Frontier *f) {
  int total = 0;
  for (int i = 0; i < MAX_THREADS; i++) {
    total += f->chunk_counts[i];
  }
  return total;
}