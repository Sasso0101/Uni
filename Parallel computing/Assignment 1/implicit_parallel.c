#include <stdio.h>
#include <stdlib.h>
#include <likwid.h>
#include <x86intrin.h>

#define SIZE 4096
#define TILE_SIZE 64


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
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < i; j++) {
            if (m[i][j] != m[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

void blocked_tranpose(float **m, float **t, int i1, int i2, int j1, int j2) {
    // Test for overflows
    i2 = (i2 < SIZE) ? i2 : SIZE;
    j2 = (j2 < SIZE) ? j2 : SIZE;
    // Perform matrix transposition on the block
    for (int i = i1; i < i2; i++) {
        // gcc thinks that this loop can't be vectorized because of a data dependence
        // since this is not the case, force gcc to ignore the data dependency check
        // by adding the ignore vector dependency pragma
        #pragma GCC ivdep
        for (int j = j1; j < j2; j++) {
            t[i][j] = m[j][i];
        }
    }
}


void divide_transpose(float **m, float **t) {
    LIKWID_MARKER_INIT;
    LIKWID_MARKER_START("transpose");
    for (int i = 0; i < SIZE; i += TILE_SIZE) {
        for (int j = 0; j < SIZE; j += TILE_SIZE) {
            blocked_tranpose(m, t, i, i + TILE_SIZE, j, j + TILE_SIZE);
        }
    }
    LIKWID_MARKER_STOP("transpose");
    LIKWID_MARKER_CLOSE;
}


int main() {
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

    // Compute blocked transpose
    divide_transpose(m, t);

    // Free memory
    free(m);
    free(t);
    return 0;
}
