#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>

#ifndef SYS_gettid
#error "SYS_gettid unavailable on this system"
#endif

#define gettid() ((pid_t)syscall(SYS_gettid))

# define FILENAME "my_file.txt"

int count;


/* puts i^2 into array positions i=start to stop-1 */
void* plus_one (void* td)
{
    int max = *(int*) td;
    pid_t my_id = gettid();
    // pid_t my_id = syscall(SYS_gettid);

    int iterations = 0;
    while(count < max){
    	count = count + 1;
    	FILE *fptr = fopen(FILENAME, "a");
    	fprintf(fptr, "thread %d has updated count to %d\n", my_id, count);
    	fclose(fptr);
    	iterations = iterations + 1;
    }
    printf("Thread %d existing after doing %d increments\n", my_id, iterations);
    return NULL;
}

int main(int argc, char *argv[]){


  if (argc != 3) {
     fprintf(stderr,"usage: a.out <n_threads> <max_value>\n");
     /*exit(1);*/
     return -1;
  }
  remove(FILENAME);
  
  int n_threads = atoi(argv[1]);
  int max_value = atoi(argv[2]);
  pthread_t thread[n_threads];
  
  struct timeval  tv1, tv2;
  gettimeofday(&tv1, NULL);
  
  count = 0;
  for(int i=0; i<n_threads;i++) {
    pthread_create(&thread[i], NULL, plus_one, &max_value);
  }

  for(int i=0; i<n_threads;i++) {
    pthread_join(thread[i], NULL);
  }

  gettimeofday(&tv2, NULL);
  double cpu_time_used = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
    
  printf("Elapsed time: took %.6f secs\n", cpu_time_used);
  printf("\n-------------------------------------------------------------------------\n");
}
