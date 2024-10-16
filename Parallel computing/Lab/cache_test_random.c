#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000000

void read_pattern(int *array, int* pattern) {
  for (int i = 0; i < N; i++) {
    int temp = array[pattern[i]];
  }
}

int main() {
	// Allocate memory for the array dynamically
  int *array = (int *)malloc(N * sizeof(int));  // Allocate rows
  int *random = (int *)malloc(N * sizeof(int));  // Allocate rows
	
  for (int i = 0; i < N; i++) {
    random[i] = rand() % N;
  }

	// Measure execution time using clock()
  clock_t start, end;
  double cpu_time_used;
	
  start = clock();  // Record the start time
  read_pattern(array, random);
  end = clock();  // Record the end time
	
	// Calculate the elapsed time in seconds
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

  printf("Time taken (random): %.4fs\n", cpu_time_used);

	// Free allocated memory
  free(array);
  free(random);

  // float bandwidth = 4*N*2/(cpu_time_used*1024*1024*1024);
  // printf("Effective bandwidth: %.3f GB/s\n", bandwidth);

  return 0;
}
