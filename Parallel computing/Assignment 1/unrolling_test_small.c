#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE 4096

void init_rand(float **m) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            m[i][j] = (float)rand();
        }
    }
}

void traverse_by_col(float **m, float **t) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            t[i][j] = m[i][j];
        }
    }
}

void traverse_by_col_2(float **m, float **t) {
  int block_size = 2;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j+=block_size) {
            t[i][j] = m[i][j];
            t[i][j+1] = m[i][j+1];
        }
    }
}

void traverse_by_col_4(float **m, float **t) {
  int block_size = 4;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j+=block_size) {
            t[i][j] = m[i][j];
            t[i][j+1] = m[i][j+1];
            t[i][j+2] = m[i][j+2];
            t[i][j+3] = m[i][j+3];
        }
    }
}

void traverse_by_col_8(float **m, float **t) {
  int block_size = 8;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j+=block_size) {
            t[i][j] = m[i][j];
            t[i][j+1] = m[i][j+1];
            t[i][j+2] = m[i][j+2];
            t[i][j+3] = m[i][j+3];
            t[i][j+4] = m[i][j+4];
            t[i][j+5] = m[i][j+5];
            t[i][j+6] = m[i][j+6];
            t[i][j+7] = m[i][j+7];
        }
    }
}

double elapsed(struct timeval start, struct timeval end) {
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds*1e-6;
    return elapsed;
}

int main() {
    float **m = (float **)malloc(SIZE * sizeof(float *));
    float **t = (float **)malloc(SIZE * sizeof(float *));
    for (int i = 0; i < SIZE; i++) {
        m[i] = (float *)malloc(SIZE * sizeof(float));
        t[i] = (float *)malloc(SIZE * sizeof(float));
    }
    init_rand(m);

    struct timeval start, end;

    gettimeofday(&start, NULL);
    traverse_by_col(m, t);
    gettimeofday(&end, NULL);
    printf("Traverse no unroll elapsed time: %.3fs\n", elapsed(start, end));

    gettimeofday(&start, NULL);
    traverse_by_col_2(m, t);
    gettimeofday(&end, NULL);
    printf("Traverse 2 unroll time: %.3fs\n", elapsed(start, end));

    gettimeofday(&start, NULL);
    traverse_by_col_4(m, t);
    gettimeofday(&end, NULL);
    printf("Traverse 4 unroll time: %.3fs\n", elapsed(start, end));

    gettimeofday(&start, NULL);
    traverse_by_col_8(m, t);
    gettimeofday(&end, NULL);
    printf("Traverse 8 unroll time: %.3fs\n", elapsed(start, end));

    free(m);
    free(t);
    return 0;
}