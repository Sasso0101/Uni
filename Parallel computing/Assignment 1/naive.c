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
    LIKWID_MARKER_START("check symmetry");
    int is_sym = 1;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < i; j++) {
            if (m[i][j] != m[j][i]) {
                is_sym = 0;
            }
        }
    }
    return is_sym;
    LIKWID_MARKER_STOP("Check symmetry");
}

void naive_transpose(float **m, float **t, int n) {
    LIKWID_MARKER_START("Naive transpose");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            t[i][j] = m[j][i];
        }
    }
    LIKWID_MARKER_STOP("Naive transpose");
}

int main() {
    LIKWID_MARKER_INIT;
    float **m = (float **)malloc(SIZE * sizeof(float *));
    float **t = (float **)malloc(SIZE * sizeof(float *));
    for (int i = 0; i < SIZE; i++) {
        m[i] = (float *)malloc(SIZE * sizeof(float));
        t[i] = (float *)malloc(SIZE * sizeof(float));
    }
    // Initialize matrix randomly
    init_rand(m);

    // Check matrix symmetry
    // int is_sym = check_sym(m);

    // Compute naive transpose
    naive_transpose(m, t, SIZE);

    // Free memory
    free(m);
    free(t);
    LIKWID_MARKER_CLOSE;
    return 0;
}
