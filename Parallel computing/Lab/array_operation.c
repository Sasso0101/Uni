#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE 500000000

void array_operations(int *a, int *b, int *c) {
    for (int i = 1; i < SIZE; i++) {
        a[i] = b[i] + b[i - 1];  // Instruction 1: True dependency on b[i - 1]
        b[i] = a[i] * 2;         // Instruction 2: Anti dependency (Write After Read on b[i])
        c[i] = a[i] + b[i - 1];  // Instruction 3: Output dependency (Write After Write on a[i])
    }
}

int main() {
    int *a = (int *)malloc(SIZE * sizeof(int));
    int *b = (int *)malloc(SIZE * sizeof(int));
    int *c = (int *)malloc(SIZE * sizeof(int));


    for (int i = 0; i < SIZE; i++) {
        a[i] = b[i] = c[i] = i;
    }
    struct timeval start, end;
    gettimeofday (& start , NULL );

    // Code to measure
    array_operations(a, b, c);

    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds*1e-6;
    printf("Elapsed time: %.6f seconds.\n", elapsed);

    free(a);
    free(b);
    free(c);
    return 0;
}
