#define _GNU_SOURCE
#include "thread_pool.h"
#include <pthread.h>
#include <sched.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void init_thread_pool(thread_pool_t *tp) {
  pthread_mutex_init(&tp->mutex, NULL);
  pthread_cond_init(&tp->cond, NULL);
  tp->allowed_threads = 0;
  tp->stop_threads = false;
}

int wait_for_work(thread_pool_t *tp) {
  pthread_mutex_lock(&tp->mutex);
  while (tp->allowed_threads == 0) {
    pthread_cond_wait(&tp->cond, &tp->mutex);
  }
  tp->allowed_threads--;
  pthread_mutex_unlock(&tp->mutex);
  if (tp->stop_threads)
    return -1;
  return 0;
}

/**
 * @brief Pins a thread to a specific logical CPU core.
 *
 * Platform-specific optimization for Linux to improve cache locality
 * and reduce contention by binding threads to specific cores.
 *
 * @param thread The pthread handle.
 * @param core The logical CPU core to bind the thread to.
 *
 * @note Only available on Linux systems. No-op on others.
 * @internal
 */
static void pin_thread_to_cpu(pthread_t thread, int core) {
#ifdef __linux__
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core, &cpuset);
  pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
#endif
}

void thread_pool_create(thread_pool_t *tp, void *(*routine)(void *)) {
  // Spawn threads
  for (int i = 0; i < MAX_THREADS; i++) {
    tp->thread_ids[i] = i;
    if (pthread_create(&tp->threads[i], NULL, routine, &tp->thread_ids[i]) !=
        0) {
      perror("Failed to create thread");
      exit(1);
    }
    // In this context CPUs in cpuset refer to logical cores, therefore the
    // optimal allocation is having one thread per logical CPU
    pin_thread_to_cpu(tp->threads[i], i);
  }
}

void join_threads(thread_pool_t *tp) {
  for (int i = 0; i < MAX_THREADS; ++i) {
    if (pthread_join(tp->threads[i], NULL) != 0) {
      perror("Failed to join thread");
      exit(1);
    }
  }
}

void thread_pool_notify(thread_pool_t *tp, bool stop_threads) {
  pthread_mutex_lock(&tp->mutex);
  tp->stop_threads = stop_threads;
  tp->allowed_threads = MAX_THREADS;
  pthread_cond_broadcast(&tp->cond);
  pthread_mutex_unlock(&tp->mutex);
}

void destroy_thread_pool(thread_pool_t *tp) {
  pthread_mutex_destroy(&tp->mutex);
  pthread_cond_destroy(&tp->cond);
}