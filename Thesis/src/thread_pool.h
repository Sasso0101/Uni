#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "config.h"
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pthread_cond_t cond;
  pthread_mutex_t mutex;
  pthread_t threads[MAX_THREADS];
  int thread_ids[MAX_THREADS];
  int allowed_threads;
  int stop_threads;
} thread_pool_t;

thread_pool_t init_thread_pool() {
  return (thread_pool_t){.cond = PTHREAD_COND_INITIALIZER,
                         .mutex = PTHREAD_MUTEX_INITIALIZER,
                         .allowed_threads = 0,
                         .stop_threads = 0};
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

void create_threads(thread_pool_t *tp, void *(*routine)(void *)) {
#ifdef __linux__
  cpu_set_t cpuset;
#endif
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
#ifdef __linux__
    CPU_ZERO(&cpuset);
    CPU_SET(i, &cpuset);
    int s = pthread_setaffinity_np(tp->threads[i], sizeof(cpuset), &cpuset);
    if (s != 0)
      perror("pthread_setaffinity_np");
#endif
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

void notify_threads(thread_pool_t *tp, int stop_threads) {
  pthread_mutex_lock(&tp->mutex);
  tp->stop_threads = stop_threads;
  tp->allowed_threads = MAX_THREADS;
  pthread_cond_broadcast(&tp->cond);
  pthread_mutex_unlock(&tp->mutex);
}

#endif