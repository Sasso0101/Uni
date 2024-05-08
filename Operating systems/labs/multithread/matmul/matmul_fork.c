#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

struct params {
    int *m1, *m2;
    int min_index, max_index, size;
    int *pipe;
};

struct result {
    int m1, m2, prod;
};

void generate_matrix(int *matrix, int size) {
    // seed values in operand matrixes
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            *(matrix + i*size + j) = rand() % 10;
        }
    }
}

void fork_worker(struct params par) {
    int sum_m1 = 0, sum_m2 = 0, sum_prod = 0;
	for(int i = par.min_index; i <= par.max_index; i++) {
        for(int j = 0; j < par.size; j++) {
            int res = 0;
            for(int k = 0; k < par.size; k++) {
        	    res += *(par.m1 + i*par.size + k) * *(par.m2 + k*par.size + j);
            }
            sum_prod += res;
            sum_m1 += *(par.m1 + i*par.size + j);
            sum_m2 += *(par.m2 + i*par.size + j);
        }
    }
    struct result res = {.m1 = sum_m1, .m2 = sum_m2, .prod = sum_prod};
    write(*((par.pipe)+1), &res, sizeof(struct result));
}

int fork_process(struct params par) {
	int pid = fork();
	if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
    } else if (pid == 0) {
		fork_worker(par);
        exit(0);
    }
	return pid;
}

// Given a size n and a number of partitions p, creates a 2D array of
// n elements with the starting and ending index of each partition
void partition_indices(int total_size, int partitions, int* indices) {
    int bucket_size = total_size / partitions;
    int current_index = 0;
    for (int i = 0; i < partitions-1; i++) {
        *(indices + 2*i) = current_index;
        current_index += bucket_size;
        *(indices + 2*i + 1) = current_index - 1;
    }
    *(indices + 2*(partitions-1)) = current_index;
    *(indices + 2*(partitions-1) + 1) = total_size - 1;
}

int main(int argc, char *argv[]) {
    // input params
    if (argc != 3) {
        fprintf(stderr, "usage: exe_name <n> <processes>\n");
        return -1;
    }
    int size = atoi(argv[1]);
    int n_processes = atoi(argv[2]);

    int matrix1[size][size];
    int matrix2[size][size];
    generate_matrix((int *)matrix1, size);
    generate_matrix((int *)matrix2, size);
    
    // Start time
    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);

    int pipes[n_processes][2];
    int indices[n_processes][2];
    partition_indices(size, n_processes, (int*)indices);
    
    for (int i = 0; i < n_processes; i++) {
        pthread_t tid;
        pthread_attr_t attr;
        pipe(pipes[i]);
        pthread_attr_init(&attr);
        struct params par = {
            .m1 = (int*) matrix1,
            .m2 = (int*) matrix2,
            .min_index = indices[i][0],
            .max_index = indices[i][1],
            .size = size,
            .pipe = pipes[i]
        };
        fork_process(par);
    }

    int sum_m1 = 0, sum_m2 = 0, sum_prod = 0;
    for (int i = 0; i < n_processes; i++) {
        struct result res;
        read(pipes[i][0], &res, sizeof(struct result));
        sum_m1 += res.m1;
        sum_m2 += res.m2;
        sum_prod += res.prod;
	}
    
    // print timings
    gettimeofday(&tv2, NULL);
    double cpu_time_used = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
    printf("Sum m1: %d, Sum m2: %d, Sum m1xm2: %d\n", sum_m1, sum_m2, sum_prod);
    printf("Elapsed time with %dx%d matrix shape: %.3f secs\n", size, size, cpu_time_used);
    
    return 0;
}