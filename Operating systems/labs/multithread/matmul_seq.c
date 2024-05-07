/*
 * Modified from 
 *
 * https://github.com/mkitzan/pthread-exercises/blob/master/matmul.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>  

// C code that performs + - * of squared matrixes that are initialized randomly
int main(int argc, char *argv[]) {

    // input params
    if (argc != 2) {
	fprintf(stderr,"usage: exe_name <n>\n");
	return -1;
    }
    int N = atoi(argv[1]);
    
    // Used vars
    int i, j, k; 
    int matrix1[N][N];
    int matrix2[N][N];
    int sum_m1, sum_m2, sum_prod;
    
    // seed values in operand matrixes
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            matrix1[i][j] = rand() % 10;
        }
    }
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            matrix2[i][j] = rand() % 10;
        }
    }
    
    // getting start time
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);
    
    // main computation
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            int res = 0;
            for(k = 0; k < N; k++) {
        	res += matrix1[i][k] * matrix2[k][j];
            }
            sum_prod += res;
            sum_m1 += matrix1[i][j];
            sum_m2 += matrix2[i][j];
        }
    }   
    
    // print timings 
    gettimeofday(&tv2, NULL);
    double cpu_time_used = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
    printf("Sum m1: %d, Sum m2: %d, Sum m1xm2: %d\n", sum_m1, sum_m2, sum_prod);
    printf("Elapsed time with %dx%d matrix shape, sequential, took %.3f secs\n", N, N, cpu_time_used);
    
    return 0;
}

