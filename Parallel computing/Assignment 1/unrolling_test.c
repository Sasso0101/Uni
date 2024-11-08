#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE 8192

void init_rand(float **m) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            m[i][j] = (float)rand();
        }
    }
}

void mat_trans(float **m, float **t) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < i + 1; j++) {
            t[j][i] = m[i][j];
            t[i][j] = m[j][i];
        }
    }
}

void mat_trans_unroll_2(float **m, float **t) {
  int block_size = 2;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < i; j+=block_size) {
            t[j][i] = m[i][j];
            t[i][j] = m[j][i];
            t[j+1][i] = m[i][j+1];
            t[i][j+1] = m[j+1][i];
        }
    }
}

void mat_trans_unroll_4(float **m, float **t) {
  int block_size = 4;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < i; j+=block_size) {
            t[j][i] = m[i][j];
            t[i][j] = m[j][i];
            t[j+1][i] = m[i][j+1];
            t[i][j+1] = m[j+1][i];
            t[j+2][i] = m[i][j+2];
            t[i][j+2] = m[j+2][i];
            t[j+3][i] = m[i][j+3];
            t[i][j+3] = m[j+3][i];
        }
    }
}

void mat_trans_unroll_8(float **m, float **t) {
  int block_size = 8;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < i; j+=block_size) {
            t[j][i] = m[i][j];
            t[i][j] = m[j][i];
            t[j+1][i] = m[i][j+1];
            t[i][j+1] = m[j+1][i];
            t[j+2][i] = m[i][j+2];
            t[i][j+2] = m[j+2][i];
            t[j+3][i] = m[i][j+3];
            t[i][j+3] = m[j+3][i];

            t[j+4][i] = m[i][j+4];
            t[i][j+4] = m[j+4][i];
            t[j+5][i] = m[i][j+5];
            t[i][j+5] = m[j+5][i];
            t[j+6][i] = m[i][j+6];
            t[i][j+6] = m[j+6][i];
            t[j+7][i] = m[i][j+7];
            t[i][j+7] = m[j+7][i];
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
    mat_trans(m, t);
    gettimeofday(&end, NULL);
    printf("Transposed elapsed time: %.3fs\n", elapsed(start, end));

    gettimeofday(&start, NULL);
    mat_trans_unroll_2(m, t);
    gettimeofday(&end, NULL);
    printf("Transposed unrolled 2 time: %.3fs\n", elapsed(start, end));

    gettimeofday(&start, NULL);
    mat_trans_unroll_4(m, t);
    gettimeofday(&end, NULL);
    printf("Transposed unrolled 4 time: %.3fs\n", elapsed(start, end));

    gettimeofday(&start, NULL);
    mat_trans_unroll_8(m, t);
    gettimeofday(&end, NULL);
    printf("Transposed unrolled 8 time: %.3fs\n", elapsed(start, end));

    free(m);
    free(t);
    return 0;
}
