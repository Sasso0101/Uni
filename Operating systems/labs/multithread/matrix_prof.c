#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
// #include <threads.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))

struct mat_sum { int m1, m2, prod; };

// To be excuted by each child
void fork_worker(int my_pipe[], int* matrix1, int* matrix2, int from, int to, int N){
	int my_pid = getpid();
	struct mat_sum thr_sum = {.m1 = 0, .m2 = 0, .prod = 0};
	// main computation
	int sum = 0;
	for(int i = from; i < to; i++) {
		for(int j = 0; j < N; j++) {
			int res = 0;	
			for(int k = 0; k < N; k++) {
				res = res + *(matrix1 + i*N + k) * *(matrix2 + k*N + j);
			}
			thr_sum.prod += res;
			thr_sum.m1 += *(matrix1 + i*N + j);
			thr_sum.m2 += *(matrix2 + i*N + j);
		}
	}   
	close(my_pipe[0]);
	write(my_pipe[1], &thr_sum, sizeof(struct mat_sum));
}


int main(int argc, char *argv[]) {

	if (argc != 3) {
		fprintf(stderr,"usage: exe_name <n> <n_threads>\n");
	     	/*exit(1);*/
	     	return -1;
	}

	int N = atoi(argv[1]);
  	int n_threads = atoi(argv[2]);
  	
  	
  	int i, j; 
	int matrix1[N][N];
	int matrix2[N][N];

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
	int pipes[n_threads][2]; 
	
	// Spawning Childs
    	for(int i=0;i<n_threads;i++){
    		pipe(pipes[i]);
	    	int pid = fork();
	    	if (pid < 0) { /* error occurred */
			fprintf(stderr, "Fork Failed\n");
			return 1;
	    	}
	    	else if (pid == 0) { /* child process, computes the first half */
			int from = i*(N/n_threads+1);
			int to = min((i+1)*(N/n_threads+1), N);
			fork_worker(pipes[i], (int *)matrix1, (int *)matrix2, from, to, N);
			return 0;
        	}
    	}
    	
    	// Wait for childs to finish and accumulate partial results
    	int sum_m1=0, sum_m2=0, sum_prod=0;
    	struct mat_sum pipe_out;
    	for(int i=0;i<n_threads;i++){
    		close(pipes[i][1]);
    		read(pipes[i][0], &pipe_out, sizeof(struct mat_sum));
    		sum_m1 += pipe_out.m1;
    		sum_m2 += pipe_out.m2;
    		sum_prod += pipe_out.prod;
    		//printf("Child #%d finished\n", i);
    	}
 
	gettimeofday(&tv2, NULL);
	double cpu_time_used = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
	printf("Sums: m1: %d, m2: %d, prod: %d\n", sum_m1, sum_m2, sum_prod);
	printf("Elapsed time: took %.3f secs\n", cpu_time_used);
    	return 0;
}

