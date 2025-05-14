#ifndef THREAD_POOL_H
#define THREAD_POOL_H

/**
 * @file thread_pool.h
 * Implementation of a simple thread pool.
 */

#include "config.h"
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <sys/types.h>

typedef struct {
  pthread_cond_t cond_children;
  pthread_mutex_t mutex_children;
  pthread_t threads[MAX_THREADS];
  int thread_ids[MAX_THREADS];
  atomic_uint_fast32_t run_id; // Counter for work cycles
  atomic_bool stop_threads;    // Flag to signal threads to terminate

  atomic_bool children_done; // Flag indicating if all threads have terminated
  pthread_cond_t cond_parent;
  pthread_mutex_t mutex_parent;

  void *(*routine)(void *); // Store the worker function pointer
} thread_pool_t;

static thread_pool_t tp;

/**
 * Initializes a thread pool structure by specifying the function pointer for
 * the task that worker threads will execute.
 */
void init_thread_pool(thread_pool_t *tp, void *(*routine)(void *));

/**
 * @brief Waits for a signal indicating new work or termination.
 *
 * Called by worker threads. Blocks the calling thread using a condition
 * variable until the main thread signals a new work cycle (by incrementing
 * `tp->run_id`) or signals termination (`tp->stop_threads`). Returns 0 if the
 * thread should proceed or calls pthread_exit directly if termination is
 * signaled.
 */
int wait_for_work(thread_pool_t *tp, uint *run_id);

/**
 * @brief Creates and launches the threads in the pool.
 *
 * Spawns `MAX_THREADS` threads. Each thread is configured to execute
 * the `thread_main_wrapper` function. The thread's index (0 to MAX_THREADS-1)
 * is passed as the argument to `thread_main_wrapper`, which is then forwarded
 * to the user's routine.
 * On Linux systems, each thread is pinned to a specific CPU core.
 */
void thread_pool_create(thread_pool_t *tp);

/**
 * Signals worker threads to start a new work cycle and waits for
 * completion.
 */
void thread_pool_start_wait(thread_pool_t *tp);

/**
 * Signals all worker threads to stop and waits for their termination.
 */
void thread_pool_terminate(thread_pool_t *tp);

/**
 * @brief Notifies the main (parent) thread that the current work cycle is
 * complete.
 *
 * Signals the
 * parent thread waiting in `thread_pool_start_wait`. This function is called by
 * a single thread after all other threads have finished their tasks for the
 * current `run_id` cycle.
 */
void thread_pool_notify_parent(thread_pool_t *tp);

/**
 * Destroys the synchronization primitives used by the thread pool.
 */
void destroy_thread_pool(thread_pool_t *tp);

#endif