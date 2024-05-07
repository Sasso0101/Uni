// Implement pthread_create() and pthread_join() using forks

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void fork_worker() {
	int pid = getpid();
	printf("Hello from child %d\n", pid);
}

int pthread_create() {
	int pid = fork();
	if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
    } else if (pid == 0) {
		fork_worker();
        exit(0);
    }
	return pid;
}

int pthread_join(int pid) {
    return waitpid(pid, NULL, WEXITED);
}

int main(int argc, char *argv[]) {
	int n_threads = atoi(argv[1]);
	int pid[n_threads];
	for (int i=0; i < n_threads; i++) {
    	pid[i] = pthread_create();
	}
	for (int i=0; i < n_threads; i++) {
		printf("Waiting for %d\n", pid[i]);
    	pthread_join(pid[i]);
		printf("Child %d got killed\n", pid[i]);
	}
	printf("Finished");
}