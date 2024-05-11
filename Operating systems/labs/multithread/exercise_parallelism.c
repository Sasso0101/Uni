/*
 * Checks properties of numbers generated randomly.
 * It counts how many numbers in the array are odd, prime or fibonacci numbers.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <omp.h>
#include <pthread.h>

#define N 30000000
#define min(a, b) (((a) < (b)) ? (a) : (b))

struct pt_struct
{
    int from, to, n_odd, n_prime, n_fib;
};

short numbers[N];

// creates shared memory matrix for forks
int *create_shared_mem(int n_threads)
{

    // Creates area of memory of size n_threads*3 integers
    int shmc = shmget(96, n_threads * 3 * sizeof(int), IPC_CREAT | 0666);
    if (shmc < 0)
    {
        printf("Asking too much memory, cant afford it!");
        exit(1);
    }
    /* Attach the segment as an int array */
    int *matrixC = shmat(shmc, NULL, 0);
    if (matrixC < (int *)NULL)
    {
        perror("shmat");
        exit(1);
    }
    return matrixC;
}

// Returns true if a number is prime
bool is_prime(int n)
{
    if (n == 0 || n == 1)
        return true;

    for (int i = 2; i <= n / 2; ++i)
    {
        if (n % i == 0)
        {
            return false;
        }
    }
    return true;
}

// returns true if n is a fibonacci number
bool is_fib(int n)
{
    int a = 0, b = 1, c = 1;
    if (n == 0 || n == 1)
        return true;
    while (c <= n)
    {
        c = a + b;
        a = b;
        b = c;
    }
    if (c == n)
        return true;
    else
        return false;
}

/* code for pthread worker */
void *pt_worker(void *td)
{
    struct pt_struct *data = (struct pt_struct *)td;
    data->n_odd = 0;
    data->n_prime = 0;
    data->n_fib = 0;
    for (int i = data->from; i < data->to; i++)
    {
        if (numbers[i] % 2 == 0)
        {
            data->n_odd++;
        }
        if (is_prime(numbers[i]))
        {
            data->n_prime++;
        }
        if (is_fib(numbers[i]))
        {
            data->n_fib++;
        }
    }
    pthread_exit(0);
}

/* code for fork worker */
void *fork_worker(int *shm, int from, int to, int child_index)
{
    int n_odd = 0;
    int n_prime = 0;
    int n_fib = 0;
    for (int i = from; i < to; i++)
    {
        if (numbers[i] % 2 == 0)
        {
            n_odd++;
        }
        if (is_prime(numbers[i]))
        {
            n_prime++;
        }
        if (is_fib(numbers[i]))
        {
            n_fib++;
        }
    }
    // Writing in the shared memory at the "row" assigned to the process, 0=odd, 1=prime, 2=fib
    *(shm + child_index * 3) = n_odd;
    *(shm + child_index * 3 + 1) = n_prime;
    *(shm + child_index * 3 + 2) = n_fib;
    exit(0);
}

// Base MAIN structure, feel free to change of you are not comfortable with that
int main(int argc, char *argv[])
{

    // -------------------------------- INPUT PROCESSING --------------------------------------
    if (argc != 2)
    {
        fprintf(stderr, "usage: scriptname <integer num_threads>\n");
        return -1;
    }
    int n_threads = atoi(argv[1]);
    int tasksPerThread = (N + n_threads - 1) / n_threads;

    // -------------------------------- INIT ARRAY --------------------------------------
    int n_odd, n_fib, n_prime;
    srand(time(0));
    for (int i = 0; i < N; i++)
    {
        numbers[i] = rand() % 100;
    }

    // -------------------------------- SEQUENTIAL CODE --------------------------------------
    // Uses 3 separate vars to compute the 3 counters
    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);

    n_odd = 0;
    n_fib = 0;
    n_prime = 0;
    for (int i = 0; i < N; i++)
    {
        if (numbers[i] % 2 == 0)
        {
            n_odd++;
        }
        if (is_prime(numbers[i]))
        {
            n_prime++;
        }
        if (is_fib(numbers[i]))
        {
            n_fib++;
        }
    }

    gettimeofday(&tv2, NULL);
    double cpu_time_used = (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
    printf("Results: %d, %d, %d sequential, took %.3f secs\n", n_odd, n_fib, n_prime, cpu_time_used);

    // -------------------------------- FORK CODE --------------------------------------
    // Hint: remember that each process has its own (local) copy of variables, thus you need something else
    // Here shared memory is used for passing partial results
    gettimeofday(&tv1, NULL);

    int *sh_mem = create_shared_mem(n_threads);
    for (int i = 0; i < n_threads; i++)
    {
        int pid = fork();
        if (pid < 0)
        { /* error occurred */
            fprintf(stderr, "Fork Failed\n");
            return 1;
        }
        else if (pid == 0)
        {
            int from = i * tasksPerThread;
            int to = min((i + 1) * tasksPerThread, N);
            fork_worker(sh_mem, from, to, i);
            return 0;
        }
    }

    // Wait for childs to finish, then go to the shmem and perform sums
    while (wait(NULL) > 0)
        ;
    n_odd = 0;
    n_fib = 0;
    n_prime = 0;
    for (int i = 0; i < n_threads; i++)
    {
        n_odd += *(sh_mem + i * 3);
        n_prime += *(sh_mem + i * 3 + 1);
        n_fib += *(sh_mem + i * 3 + 2);
    }

    gettimeofday(&tv2, NULL);
    cpu_time_used = (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
    printf("Results: %d, %d, %d with %d subprocesses, took %.3f secs\n", n_odd, n_fib, n_prime, n_threads, cpu_time_used);

    // -------------------------------- PTHREADS CODE --------------------------------------
    // Hint: dont forget the -lpthread flag while compiling
    // the input struct also contains placeholders for output
    gettimeofday(&tv1, NULL);

    pthread_t thread[n_threads];
    struct pt_struct pt_in[n_threads];
    for (int i = 0; i < n_threads; i++)
    {
        pt_in[i].from = i * tasksPerThread;
        pt_in[i].to = min((i + 1) * tasksPerThread, N);
        pthread_create(&thread[i], NULL, pt_worker, &pt_in[i]);
    }

    n_odd = 0;
    n_fib = 0;
    n_prime = 0;
    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(thread[i], NULL);
        n_odd += pt_in[i].n_odd;
        n_prime += pt_in[i].n_prime;
        n_fib += pt_in[i].n_fib;
    }

    gettimeofday(&tv2, NULL);
    cpu_time_used = (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
    printf("Results: %d, %d, %d with %d pthreads, took %.3f secs\n", n_odd, n_fib, n_prime, n_threads, cpu_time_used);

    // -------------------------------- OMP CODE --------------------------------------
    // Hint: dont forget the -fopenmp flag, and to set the number of threads either in code or in the shell
    gettimeofday(&tv1, NULL);

    n_odd = 0;
    n_fib = 0;
    n_prime = 0;
    // this is required if you want to set the number of thread in the code and not in the shell
    omp_set_dynamic(0);
// the num_threads specified the omp threads, the reduction clause is useful in the loops that sum onto a specific variable
#pragma omp parallel for num_threads(n_threads) reduction(+ : n_odd, n_fib, n_prime)
    for (int i = 0; i < N; i++)
    {
        if (numbers[i] % 2 == 0)
        {
            n_odd++;
        }
        if (is_prime(numbers[i]))
        {
            n_prime++;
        }
        if (is_fib(numbers[i]))
        {
            n_fib++;
        }
    }
    gettimeofday(&tv2, NULL);
    cpu_time_used = (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
    printf("Results: %d, %d, %d with %d omp threads, took %.3f secs\n", n_odd, n_fib, n_prime, n_threads, cpu_time_used);

    return 0;
}
