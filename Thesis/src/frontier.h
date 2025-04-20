#ifndef FRONTIER_H
#define FRONTIER_H

/**
 * @brief Thread-local vertex chunk pool for parallel graph traversal.
 *
 * The Frontier data structure manages per-thread pools of reusable vertex
 * chunks for use in parallel graph algorithms. Each thread maintains its own
 * chunk pool to minimize contention and improve cache locality.
 *
 * ## Design Overview
 * - The Frontier is composed of MAX_THREADS thread-local chunk pools.
 * - Each pool (`ChunkPool`) contains a dynamically resizable array of
 *   pointers to `VertexChunk` blocks.
 * - A `VertexChunk` holds a fixed-size array of vertices (CHUNK_SIZE) and acts
 *   as a LIFO (stack-style) buffer.
 * - Threads acquire and release chunks from their own pool in a lock-protected
 * manner.
 * - Chunks are not freed when released — they are reused, minimizing dynamic
 * allocations.
 *
 * ## Concurrency Model
 * - Chunk acquisition and release are thread-safe for individual threads (each
 * thread locks only its own pool).
 * - No synchronization is required for accessing the internal content of a
 * `VertexChunk` if it is used exclusively by one thread.
 * - Cross-thread stealing is supported via inspection functions.
 */

#include "config.h"
#include <pthread.h>
#include <stdint.h>

typedef struct {
  mer_t vertices[CHUNK_SIZE];
  int next_free_index;
} VertexChunk;

typedef struct {
  VertexChunk **chunks;
  int chunks_size;
  int initialized_count;
  int next_free_chunk;
  pthread_mutex_t lock;
} ChunkPool;

typedef struct {
  ChunkPool **thread_chunks;
  int *chunk_counts;
} Frontier;

/**
 * @brief Creates and initializes a new Frontier structure.
 *
 * Allocates memory for per-thread vertex chunk pools and sets up mutexes
 * for thread-safe chunk acquisition and release.
 *
 * @return Pointer to a newly created Frontier, or NULL if allocation fails.
 *
 * @note This function is not thread-safe and should be called during
 * single-threaded initialization.
 */
Frontier *frontier_create();

/**
 * @brief Destroys a Frontier structure and deallocates all associated
 * resources.
 *
 * This includes freeing all thread-local chunk arrays and the chunks
 * themselves, as well as cleaning up associated mutexes.
 *
 * @param f Pointer to the Frontier to destroy.
 *
 * @note This function is not thread-safe. Ensure all threads have finished
 * using the Frontier.
 */
void destroy_frontier(Frontier *f);

/**
 * @brief Acquires a VertexChunk for a given thread.
 *
 * Retrieves a chunk from the thread's chunk pool. If the pool has no
 * initialized chunks available, a new chunk is allocated and added to the pool.
 *
 * @param f Pointer to the Frontier structure.
 * @param thread_id ID of the calling thread (used to index into thread-local
 * pools).
 *
 * @return Pointer to an acquired VertexChunk.
 *
 * @note This function is thread-safe for the specified thread_id.
 */
VertexChunk *frontier_acquire_chunk(Frontier *f, int thread_id);

/**
 * @brief Releases the most recently acquired VertexChunk back to the pool.
 *
 * Marks the last acquired chunk as no longer in use, making it available for
 * reuse. Does not deallocate memory.
 *
 * @param f Pointer to the Frontier structure.
 * @param thread_id ID of the thread releasing the chunk.
 *
 * @return Pointer to the released VertexChunk, or NULL if the pool is empty.
 *
 * @note This function is thread-safe for the specified thread_id.
 */
VertexChunk *frontier_release_chunk(Frontier *f, int thread_id);

/**
 * @brief Pushes a vertex into the given VertexChunk.
 *
 * Adds a vertex to the next available position in the chunk.
 * Asserts that the chunk is valid and not full.
 *
 * @param c Pointer to the VertexChunk.
 * @param v Vertex to insert.
 *
 * @note This function is not thread-safe. Caller must ensure exclusive access.
 */
void chunk_push_vertex(VertexChunk *c, mer_t v);

/**
 * @brief Pops the most recently inserted vertex from the chunk.
 *
 * Removes and returns the vertex at the top of the chunk (LIFO order).
 *
 * @param c Pointer to the VertexChunk.
 *
 * @return The removed vertex value, or UINT32_MAX if the chunk is empty.
 *
 * @note This function is not thread-safe. Caller must ensure exclusive access.
 */
mer_t chunk_pop_vertex(VertexChunk *c);

/**
 * @brief Identifies the thread with the most allocated chunks.
 *
 * Returns the thread ID of a thread with more than one chunk in use.
 * Used as a heuristic for work-stealing, avoiding cache locality degradation.
 *
 * @param f Pointer to the Frontier structure.
 *
 * @return Thread ID with the most reusable chunks, or -1 if none found.
 *
 * @note This function is not thread-safe. May return inconsistent results if
 * called concurrently.
 */
int frontier_get_stealable_thread(Frontier *f);

/**
 * @brief Calculates the total number of in-use chunks across all threads.
 *
 * @param f Pointer to the Frontier structure.
 *
 * @return Total number of active chunks in the Frontier.
 *
 * @note This function is not thread-safe.
 */
int frontier_get_total_chunks(Frontier *f);

#endif // FRONTIER_H