#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000000

int main(int argc, char** argv) {
    int istart, iend, rank, size = 0;
    double time1, time2;

    double* vector_a = (double*)malloc(sizeof(double) * N);
    double* vector_b = (double*)malloc(sizeof(double) * N);

    // Initialize vectors
    for (int i = 0; i < N; ++i) {
        vector_a[i] = 1.0;
        vector_b[i] = 2.0;
    }
	
	double dot_product, partial_sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    time1 = MPI_Wtime();

    istart = (N/size) * rank;
    iend = (N/size) * (rank + 1) - 1;
    if (iend > N) {
        iend = N;
    }

    // Calculate dot product serially
    for (int i = istart; i < iend; ++i) {
        partial_sum += vector_a[i] * vector_b[i];
    }

    MPI_Reduce(&partial_sum, &dot_product, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    time2 = MPI_Wtime();

    printf("Rank %d: %f\n", rank, partial_sum);

    if (rank == 0) {
        printf("Time taken: %f\n", time2 - time1);
        printf("Parallel Dot Product: %f\n", dot_product);
    }

    MPI_Finalize();
    return 0;
}

