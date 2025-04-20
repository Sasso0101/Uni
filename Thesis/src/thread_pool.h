#ifndef THREAD_POOL_H
#define THREAD_POOL_H

/**
 * @file thread_pool.h
 * @brief Lightweight thread pool for parallel task execution.
 *
 * This module provides a simple thread pool implementation tailored
 * for fixed-size parallel workloads. It supports creation of a fixed number of
 * threads (defined by MAX_THREADS) and a signaling mechanism to notify threads
 * when work is available.
 *
 * ## Design Overview
 * - Threads are created once and pinned to specific CPU cores (on Linux).
 * - Each thread waits on a condition variable until notified via
 * `thread_pool_notify`. Optionally, a `stop_threads` flag can be passed to the
 * threads.
 * - `wait_for_work` allows threads to block efficiently until work is
 * permitted. The user is responsible of terminating the thread when the
 * `stop_threads` flag is set.
 */

#include "config.h"
#include <pthread.h>
#include <stdbool.h>

typedef struct {
  pthread_cond_t cond;
  pthread_mutex_t mutex;
  pthread_t threads[MAX_THREADS];
  int thread_ids[MAX_THREADS];
  int allowed_threads;
  bool stop_threads;
} thread_pool_t;

/**
 * @brief Initializes the thread pool structure.
 *
 * Initializes the condition variable and mutex used for thread synchronization.
 * Must be called before using any other thread pool functions.
 *
 * @param tp Pointer to the thread pool structure.
 */
void init_thread_pool(thread_pool_t *tp);

/**
 * @brief Waits until the thread pool signals work.
 *
 * Blocks the calling thread until it is allowed to proceed (signaled via
 * condition variable). Decrements the `allowed_threads` counter upon waking.
 *
 * @param tp Pointer to the thread pool.
 * @return 0 if work is allowed, -1 if the thread pool has been stopped.
 */
int wait_for_work(thread_pool_t *tp);

/**
 * @brief Creates and starts all threads in the thread pool.
 *
 * Spawns MAX_THREADS threads and optionally pins them to logical CPU cores.
 * Each thread is passed a pointer to its unique thread ID, starting from 0.
 *
 * @param tp Pointer to the thread pool.
 * @param routine Function to run in each thread (must match pthread signature).
 *
 * @note Not thread-safe. Should be called once during setup.
 * @warning Exits the program on failure to create a thread.
 */
void thread_pool_create(thread_pool_t *tp, void *(*routine)(void *));

/**
 * @brief Waits for all threads in the pool to finish execution.
 *
 * Joins each thread and blocks until they all exit.
 *
 * @param tp Pointer to the thread pool.
 *
 * @note Not thread-safe. Should be used during shutdown.
 * @warning Exits the program on failure to join a thread.
 */
void join_threads(thread_pool_t *tp);

/**
 * @brief Wakes all worker threads in the pool.
 *
 * Sets the number of allowed threads and optionally signals all threads to
 * stop. Broadcasts to the condition variable to wake up all waiting threads.
 *
 * @param tp Pointer to the thread pool.
 * @param stop_threads Whether threads should terminate after being woken up.
 *
 * @note Thread-safe.
 */
void thread_pool_notify(thread_pool_t *tp, bool stop_threads);

/**
 * @brief Destroys the thread pool and its synchronization primitives.
 *
 * Cleans up internal resources (mutexes and condition variables).
 * Frees the memory allocated for the thread pool structure.
 *
 * @param tp Pointer to the thread pool (must have been allocated with malloc).
 *
 * @note Not thread-safe. Should be called after all threads are joined.
 */
void destroy_thread_pool(thread_pool_t *tp);

#endif