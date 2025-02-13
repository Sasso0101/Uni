#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TIMER_START S=get_timestamp()
#define TIMER_STOP E=get_timestamp()

double get_timestamp() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1.e-9;
}

int size = 4096;

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
    for (int i = 0; i < size; i++) {
        // gcc thinks that this loop can't be vectorized because of a data dependence
        // since this is not the case, force gcc to ignore the data dependency check
        // by adding the ignore vector dependency pragma
        #pragma GCC ivdep
        for (int j = 0; j < size; j++) {
            t[i][j] = m[j][i];
        }
    }
}

int main(int argc, char **argv) {
    if (argc > 1) {
        size = strtol(argv[1], NULL, 10);
    }

    // Run the transpose multiple times to get an average
    float **m = (float **)malloc(size * sizeof(float *));
    float **t = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; i++) {
        m[i] = (float *)malloc(size * sizeof(float));
        t[i] = (float *)malloc(size * sizeof(float));
    }
    // Initialize matrix randomly until it is not symmetric
    do {
        init_rand(m);
    } while (check_sym(m));

    // Check matrix symmetry
    // int is_sym = check_sym(m);

    double S=0.0, E=0.0;
    TIMER_START;
    // Compute naive transpose
    transpose(m, t);
    TIMER_STOP;

    // Print wall time
    printf("%f\n", E-S);

    // Free memory
    free(m);
    free(t);

    return 0;
}
