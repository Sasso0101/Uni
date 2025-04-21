#define _GNU_SOURCE
#include "thread_pool.h"
#include <pthread.h>
#include <sched.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void init_thread_pool(thread_pool_t *tp, void *(*routine)(void *)) {
  pthread_mutex_init(&tp->mutex_children, NULL);
  pthread_cond_init(&tp->cond_children, NULL);
  pthread_mutex_init(&tp->mutex_parent, NULL);
  pthread_cond_init(&tp->cond_parent, NULL);
  tp->run_id = 0;
  tp->stop_threads = false;
  tp->children_done = false;
  tp->routine = routine;
}

int wait_for_work(thread_pool_t *tp, uint *run_id) {
  pthread_mutex_lock(&tp->mutex_children);
  while (tp->run_id < *run_id) {
    pthread_cond_wait(&tp->cond_children, &tp->mutex_children);
  }
  *run_id += 1;
  pthread_mutex_unlock(&tp->mutex_children);
  if (tp->stop_threads) {
    pthread_exit(0);
  }
  return 0;
}

void *thread_main_wrapper(void *arg) {
  uint run_id = 1;
  while (wait_for_work(&tp, &run_id) == 0) {
    tp.routine(arg);
  }
  return NULL;
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

void thread_pool_create(thread_pool_t *tp) {
  // Spawn threads
  for (int i = 0; i < MAX_THREADS; i++) {
    tp->thread_ids[i] = i;
    if (pthread_create(&tp->threads[i], NULL, thread_main_wrapper,
                       &tp->thread_ids[i]) != 0) {
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

void thread_pool_terminate(thread_pool_t *tp) {
  pthread_mutex_lock(&tp->mutex_children);
  tp->stop_threads = true;
  tp->run_id++;
  pthread_cond_broadcast(&tp->cond_children);
  pthread_mutex_unlock(&tp->mutex_children);
  join_threads(tp);
}

void thread_pool_start_wait(thread_pool_t *tp) {
  pthread_mutex_lock(&tp->mutex_children);
  pthread_mutex_lock(&tp->mutex_parent);
  tp->run_id++;
  tp->children_done = false;

  pthread_cond_broadcast(&tp->cond_children);
  pthread_mutex_unlock(&tp->mutex_children);
  while (!tp->children_done) {
    pthread_cond_wait(&tp->cond_parent, &tp->mutex_parent);
  }
  pthread_mutex_unlock(&tp->mutex_parent);
}

void thread_pool_notify_parent(thread_pool_t *tp) {
  pthread_mutex_lock(&tp->mutex_parent);
  tp->children_done = true;
  pthread_cond_signal(&tp->cond_parent);
  pthread_mutex_unlock(&tp->mutex_parent);
}

void destroy_thread_pool(thread_pool_t *tp) {
  pthread_mutex_destroy(&tp->mutex_children);
  pthread_cond_destroy(&tp->cond_children);
  pthread_mutex_destroy(&tp->mutex_parent);
  pthread_cond_destroy(&tp->cond_parent);
}