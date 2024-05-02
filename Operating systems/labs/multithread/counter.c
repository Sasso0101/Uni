#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

#define FILENAME "count.txt"

/* Write a sequential program that takes an int n and prints the count 1..n in a file */

struct increment_params {
    int max_value;
    pthread_mutex_t lock;
};

pthread_mutex_t lock;
int count;

void* increment(void *max_value) {
    int done = 0;
    uint64_t thread_id;
    pthread_threadid_np(NULL, &thread_id);
    while (done == 0) {
        pthread_mutex_lock(&lock);
        if (count < *(int*)max_value) {
            count += 1;
            FILE *fptr = fopen(FILENAME, "a");
            fprintf(fptr, "thread %d has updated count to %d\n", thread_id, count);
            fclose(fptr);
        } else {
            done = 1;
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: <num_threads> <count>");
        return -1;
    }

    int n_threads = atoi(argv[1]);
    int max_value = atoi(argv[2]);
    pthread_t thread[n_threads];
    count = 0;
    pthread_mutex_init(&lock, NULL);

    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);

    for (int i = 0; i < n_threads; i++) {
        pthread_create(&thread[i], NULL, increment, &max_value);
    }
    for (int i = 0; i < n_threads; i++) {
        pthread_join(thread[i], NULL);
    }

    gettimeofday(&tv2, NULL);
    double cpu_time_used = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
    printf("Elapsed time: %.6f secs\n", cpu_time_used);

    return 0;
}