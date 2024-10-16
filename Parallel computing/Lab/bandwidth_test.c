#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000000

void write(int *array) {
    for (int i = 0; i < N; i++) {
      array[i] = i;
    }
}

void read(int *array) {
    for (int i = 0; i < N; i++) {
      int value = array[i];
    }
}

int main() {
	// Allocate memory for the array dynamically
  int *array = (int *)malloc(N * sizeof(int));  // Allocate rows
	
	// Measure execution time using clock()
  clock_t start, end;
  double cpu_time_used;
	
	start = clock();  // Record the start time
  write(array);
  read(array);
  end = clock();  // Record the end time
	
	 // Calculate the elapsed time in seconds
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

  printf("Time taken: %.4fs\n", cpu_time_used);

	// Free allocated memory
  free(array);  // Free the row pointers

  float bandwidth = 2*sizeof(int)*N/(cpu_time_used*1024*1024);
  printf("Effective bandwidth: %.3f MB/s\n", bandwidth);

  return 0;
}
