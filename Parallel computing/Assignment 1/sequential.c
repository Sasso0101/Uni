#include <stdio.h>
#include <stdlib.h>
#include <likwid.h>

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

void transpose(float **m, float **t) {
    int a;
    LIKWID_MARKER_INIT;
    LIKWID_MARKER_START("Transpose");
    for (int i = 0; i < SIZE; i++) {
        #pragma GCC ivdep
        for (int j = 0; j < SIZE; j++) {
            t[i][j] = m[j][i];
        }
    }
    LIKWID_MARKER_STOP("Transpose");
    LIKWID_MARKER_CLOSE;
}

int main() {
    float **m = (float **)malloc(SIZE * sizeof(float *));
    float **t = (float **)malloc(SIZE * sizeof(float *));
    for (int i = 0; i < SIZE; i++) {
        m[i] = (float *)malloc(SIZE * sizeof(float));
        t[i] = (float *)malloc(SIZE * sizeof(float));
    }
    init_rand(m);

    // int is_sym = check_sym(m);
    // printf("Symmetry: %d\n\n", is_sym);

    transpose(m, t);
    printf("out: %f\n", t[1000]);
    free(m);
    free(t);
    return 0;
}
