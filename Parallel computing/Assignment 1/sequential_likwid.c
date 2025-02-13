#include <stdio.h>
#include <stdlib.h>
#include <likwid.h>

int size = 4096;
int runs = 2;

void init_rand(float **m) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            m[i][j] = (float)rand();
        }
    }
}

void print_mat(float **m) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%12.0f ", m[i][j]);
        }
        printf("\n");
    }
}

int check_sym(float **m) {
    int is_sym = 1;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < i; j++) {
            if (m[i][j] != m[j][i]) {
                is_sym = 0;
            }
        }
    }
    return is_sym;
}

void transpose(float **m, float **t) {
    LIKWID_MARKER_INIT;
    LIKWID_MARKER_START("transpose");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            t[i][j] = m[j][i];
        }
    }
    LIKWID_MARKER_STOP("transpose");
    LIKWID_MARKER_CLOSE;
}

int main(int argc, char **argv) {
    if (argc > 2) {
        size = strtol(argv[1], NULL, 10);
    }

    // Run the transpose multiple times to get an average
    for (int run = 0; run < runs; run++) {
        float **m = (float **)malloc(size * sizeof(float *));
        float **t = (float **)malloc(size * sizeof(float *));
        for (int i = 0; i < size; i++) {
            m[i] = (float *)malloc(size * sizeof(float));
            t[i] = (float *)malloc(size * sizeof(float));
        }
        // Initialize matrix randomly
        init_rand(m);

        // Check matrix symmetry
        // int is_sym = check_sym(m);

        // Compute naive transpose
        transpose(m, t);
        // Free memory
        free(m);
        free(t);
    }

    return 0;
}
