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

void print_mat(float **m) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%12.0f ", m[i][j]);
        }
        printf("\n");
    }
}

int check_sym(float **m) {
    int is_sym = 1;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < i; j++) {
            if (m[i][j] != m[j][i]) {
                is_sym = 0;
            }
        }
    }
    return is_sym;
}

void mat_trans(float **m, float **t) {
    int a;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            t[i][j] = m[j][i];
        }
    }
}

double calc_elapsed(struct timeval start, struct timeval end) {
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
    // gettimeofday(&start, NULL);
    // int is_sym = check_sym(m);
    // gettimeofday(&end, NULL);
    // printf("Elapsed time: %.3fs\n", calc_elapsed(start, end));
    // printf("Symmetry: %d\n\n", is_sym);

    gettimeofday(&start, NULL);
    mat_trans(m, t);
    gettimeofday(&end, NULL);
    double elapsed = calc_elapsed(start, end);
    printf("Elapsed time: %.3fs\n", elapsed);
    printf("Bandwidth: %.2fMB/s\n", (sizeof(float)*2*(double)SIZE*SIZE)/(elapsed*1e6));

    free(m);
    free(t);
    return 0;
}
