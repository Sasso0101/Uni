#ifndef FRONTIER_H
#define FRONTIER_H

/**
 * @brief Frontier implementation for parallel BFS traversal.
 *
 * The Frontier data structure manages per-thread pools of reusable vertex
 * chunks.
 *
 * ## Design Overview
 * - The Frontier is composed of MAX_THREADS thread-local chunk pools.
 * - Each pool (`ThreadChunks`) contains a dynamically resizable array of
 *   pointers to `Chunk` blocks.
 * - A `Chunk` holds a fixed-size array of vertices (CHUNK_SIZE) and acts
 *   as a LIFO buffer.
 * - Threads acquire and release chunks from their own pool in a lock-protected
 * manner.
 * - Chunks are reused, minimizing new allocations.
 *
 * ## Concurrency Model
 * - Chunk acquisition and release are thread-safe for individual threads (each
 * thread locks only its own pool).
 * - No synchronization is required for accessing the internal content of a
 * `Chunk` if it is used exclusively by one thread.
 */

#include "config.h"
#include <pthread.h>
#include <stdint.h>

typedef struct {
  mer_t vertices[CHUNK_SIZE];
  int next_free_index;
} Chunk;

typedef struct {
  Chunk **chunks;
  int chunks_size;
  int initialized_count;
  int next_free_chunk;
  pthread_mutex_t lock;
} ThreadChunks;

typedef struct {
  ThreadChunks **thread_chunks;
  int *chunk_counts;
} Frontier;

/**
 * @brief Creates and initializes a new Frontier structure.
 *
 * Allocates memory for per-thread vertex chunk pools and sets up mutexes
 * for thread-safe chunk acquisition and release.
 */
Frontier *frontier_create();

/**
 * @brief Destroys a Frontier structure and deallocates all associated
 * resources.
 *
 * This includes freeing all thread-local chunk arrays and the chunks
 * themselves, as well as cleaning up associated mutexes.
 */
void frontier_destroy(Frontier *f);

/**
 * @brief Acquires a Chunk for a given thread.
 *
 * Retrieves the first empty chunk for the specified thread id. If the
 * thread has no initialized chunks available, a new chunk is allocated.
 */
Chunk *frontier_create_chunk(Frontier *f, int thread_id);

/**
 * @brief Removes the top Chunk of the specified thread_id.
 *
 * Marks the last acquired chunk as no longer in use, making it available for
 * reuse. Does not deallocate memory.
 */
Chunk *frontier_remove_chunk(Frontier *f, int thread_id);

/**
 * Pushes a vertex into the given VertexChunk.
 */
void chunk_push_vertex(Chunk *c, mer_t v);

/**
 * Pops the most recently inserted vertex from the chunk. Returns UINT32_MAX if
 * the chunk is empty.
 */
mer_t chunk_pop_vertex(Chunk *c);

/**
 * @brief Identifies the thread with the most allocated chunks.
 *
 * Returns the thread ID of a thread with more than one chunk in use.
 * Used as a heuristic for work-stealing.
 */
int frontier_get_stealable_thread(Frontier *f);

/**
 * Calculates the total number of in-use chunks across all threads.
 */
int frontier_get_total_chunks(Frontier *f);

#endif // FRONTIER_H