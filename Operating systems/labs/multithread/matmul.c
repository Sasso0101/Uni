/*
 * Modified from 
 *
 * https://github.com/mkitzan/pthread-exercises/blob/master/matmul.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

void generate_matrix(int *matrix, int size) {
    // seed values in operand matrixes
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            *(matrix + i*size + j) = rand() % 10;
        }
    }
}

void fork_worker(int* matrix1, int* matrix2, int min_index, int max_index, int size, int pipe[]) {
    int sum_m1, sum_m2, sum_prod;
	for(int i = min_index; i < max_index; i++) {
        for(int j = 0; j < size; j++) {
            int res = 0;
            for(int k = 0; k < size; k++) {
        	    res += *(matrix1 + i*size + k) * *(matrix2 + i*size + j);
            }
            sum_prod += res;
            sum_m1 += *(matrix1 + i*size + j);
            sum_m2 += *(matrix2 + i*size + j);
        }
    }
    write(pipe[1], &sum_prod, sizeof(int));
    write(pipe[1], &sum_m1, sizeof(int));
    write(pipe[1], &sum_m2, sizeof(int));
    printf("sent %d %d %d\n", sum_m1, sum_m2, sum_prod);
}

int fork_process(int matrix1[], int matrix2[], int min_index, int max_index, int size, int pipe[]) {
	int pid = fork();
	if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
    } else if (pid == 0) {
		fork_worker(matrix1, matrix2, min_index, max_index, size, pipe);
        exit(0);
    }
	return pid;
}

// C code that performs + - * of squared matrixes that are initialized randomly
int main(int argc, char *argv[]) {

    // input params
    if (argc != 3) {
	fprintf(stderr,"usage: exe_name <n> <processes>\n");
	return -1;
    }
    int size = atoi(argv[1]);
    int matrix1[size][size];
    int matrix2[size][size];

    generate_matrix((int *)matrix1, size);
    generate_matrix((int *)matrix2, size);
    
    // Start time
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);

    int n_processes = atoi(argv[2]);
    int pid[n_processes];
    int pipes[n_processes][2];
    
    for (int i = 0; i < n_processes-1; i++) {
        pipe(pipes[i]);
        int pid = fork_process((int *)matrix1, (int *)matrix2, size/n_processes*i, (size/n_processes*(i+1))-1, size, pipes[i]);
    }
    pipe(pipes[n_processes-1]);
    pid[n_processes-1] = fork_process((int *)matrix1, (int *)matrix2, size/n_processes*(n_processes-1), size, size, pipes[n_processes-1]);

    int sum_m1, sum_m2, sum_prod;
    for (int i=0; i < n_processes; i++) {
        int sum_m1a, sum_m2a, sum_proda;
        char inbuf[sizeof(int)];
        read(pipes[i][0], inbuf, sizeof(int));
        sum_m1a = atoi(inbuf);

        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);
        // sum_m2a = read(pipes[i][0], inbuf, sizeof(int));
        // sum_m2a = atoi(inbuf);
        // sum_proda = read(pipes[i][0], inbuf, sizeof(int));
        // sum_proda = atoi(inbuf);
        sum_m1 += sum_m1a;
        sum_m2 += sum_m2a;
        sum_prod += sum_prod;
        printf("received from %d %d %d %d\n", i, sum_m1a, sum_m2a, sum_proda);
	}
    
    // print timings
    gettimeofday(&tv2, NULL);
    double cpu_time_used = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
    printf("Sum m1: %d, Sum m2: %d, Sum m1xm2: %d\n", sum_m1, sum_m2, sum_prod);
    printf("Elapsed time with %dx%d matrix shape, parallel, took %.3f secs\n", size, size, cpu_time_used);
    
    return 0;
}

