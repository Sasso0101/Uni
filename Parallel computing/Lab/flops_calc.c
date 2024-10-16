#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 2048

void matrix_calculation(float **a, float**b, float** res) {
    for (int i = 0; i < N; i++) {
        float sum;
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                sum += a[i][k]*b[k][i];
            }
            res[i][j] = sum;
        }
    }
}

int main() {
	// Allocate memory for the matrix dynamically
    float **a = (float **)malloc(N * sizeof(float *));
    float **b = (float **)malloc(N * sizeof(float *));
    float **res = (float **)malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++) {
        a[i] = (float *)malloc(N * sizeof(float));
        b[i] = (float *)malloc(N * sizeof(float));
        res[i] = (float *)malloc(N * sizeof(float));
    }
	// Initialize the matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a[i][j] = i * 1.0;
            b[i][j] = i * 2.0;
        }
    }
	
	// Measure execution time using clock()
    clock_t start, end;
    double cpu_time_used;
	
	start = clock();  // Record the start time
    matrix_calculation(a, b, res);
	end = clock();  // Record the end time
	
	 // Calculate the elapsed time in seconds
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("%.4fs\n", cpu_time_used);

    double bandwidth = (N*N+N*N*(2*sizeof(int)*(double)N))/(cpu_time_used*1024*1024);
    double flops = (2*N*N*(double)N)/(cpu_time_used*1e6);
    printf("Bandwidth: %.3f MB/s\n", bandwidth);
    printf("FLOPS: %.3f MFLOPs/s\n", flops);

	// Free allocated memory
    for (int i = 0; i < N; i++) {
        free(a[i]);
        free(b[i]);
        free(res[i]);
    }
    free(a);
    free(b);
    free(res);

    return 0;
}
