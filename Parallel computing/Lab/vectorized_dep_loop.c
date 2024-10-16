#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10000000

// Original loop with dependencies
void dep_loop(float *a, float *b) {
    for (int i = 1; i < SIZE; i++) {
        a[i] = a[i - 1] + b[i];  // Use of `a[i - 1]` creates an assumed dependency
    }
}

// Function using `#pragma ivdep` and `#pragma unroll` to ignore dependencies and unroll
void ivdep_loop(float *a, float *b) {
    #pragma ivdep         // Ignore assumed dependencies
    for (int i = 1; i < SIZE; i++) {
        a[i] = a[i - 1] + b[i];  // Use of `a[i - 1]` creates an assumed dependency
    }
}

int main() {
    float *a = (float *)malloc(SIZE * sizeof(float));
    float *b = (float *)malloc(SIZE * sizeof(float));

    for (int i = 0; i < SIZE; i++) {
        a[i] = i * 1.0f;
        b[i] = (i + 1) * 1.0f;
    }

	clock_t start = clock();
    dep_loop(a, b);
    clock_t end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken by the loop with dependencies: %f seconds\n", time_taken);

	// Re-initialize `a`
    for (int i = 0; i < SIZE; i++) {
        a[i] = i * 1.0f;
    }
	
	start = clock();
    ivdep_loop(a, b);
    end = clock();
    time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken by the ivdep loop: %f seconds\n", time_taken);

    // Free dynamically allocated memory
    free(a);
    free(b);

    return 0;
}

