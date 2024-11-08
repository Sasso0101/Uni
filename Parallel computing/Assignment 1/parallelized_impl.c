#include <stdio.h>
#include <stdlib.h>
#include <likwid.h>

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

void mat_trans(float **m, float **t, int i1, int i2, int j1, int j2) {
    for (int i = i1; i < i2; i++) {
        for (int j = j1; j < j2; j++) {
            t[i][j] = m[j][i];
        }
    }
}


void divide(float **m, float **t, int i1, int i2, int j1, int j2) {
    LIKWID_MARKER_INIT;
    LIKWID_MARKER_START("Transposition");
    for (int i = 0; i < SIZE; i += TILE_SIZE) {
        for (int j = 0; j < SIZE; j += TILE_SIZE) {
            mat_trans(m, t, i, i + TILE_SIZE, j, j + TILE_SIZE);
        }
    }
    LIKWID_MARKER_STOP("Transposition");
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

    struct timeval start, end;
    // int is_sym = check_sym(m);
    divide(m, t, 0, SIZE, 0, SIZE);
    free(m);
    free(t);
    return 0;
}
