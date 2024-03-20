#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

const int N = 100;

void generator(int* array, int size) {
	for (int i = 0; i < size; i++) {
		array[i] = rand() % 999;
		printf("[%d] %d\n", i, array[i]);
	}
}

float sum_array(int* array, int size) {
	clock_t t;
	float sum = 0;
    t = clock();
	for (int i = 0; i < size; i++) {
		sum += array[i];
	}
	t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
	printf("Time taken: %fs\n", time_taken);
	return sum;
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	int size = atoi(argv[1]);
	int* array1 = (int*) malloc(sizeof(int)*size);
	int* array2 = (int*) malloc(sizeof(int)*size);
	generator(array1, size);
	generator(array2, size);
	float sum = sum_array(array1, size) + sum_array(array2, size);
	printf("Sum: %.0f\n", sum);
}