#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 15000

void matrix_calculation(int **matrix) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = 4;
        }
    }
}

int main() {
	// Allocate memory for the matrix dynamically
    int **matrix = (int **)malloc(N * sizeof(int *));  // Allocate rows
    for (int i = 0; i < N; i++) {
        matrix[i] = (int *)malloc(N * sizeof(int));    // Allocate columns for each row
    }
	// Initialize the matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = 0;
        }
    }
	
	// Measure execution time using clock()
    clock_t start, end;
    double cpu_time_used;
	
	start = clock();  // Record the start time
    matrix_calculation(matrix);
	end = clock();  // Record the end time
	
	 // Calculate the elapsed time in seconds
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("%f\n", cpu_time_used);

	// Free allocated memory
    for (int i = 0; i < N; i++) {
        free(matrix[i]);  // Free each row
    }
    free(matrix);  // Free the row pointers

    return 0;
}
