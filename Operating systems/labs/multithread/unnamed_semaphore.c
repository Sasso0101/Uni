// Unnamed semaphore
#include <semaphore.h>
int main() {
    sem_t *sem;
    sem = sem_open("SEM", O_CREAT, 0666, 1);
    sem_wait(&sem);
    // Critical section
    sem_post(&sem);
}

// Named semaphore
int main() {
    sem_t sem;
    sem_init(&sem, 0, 1);
    sem_wait(&sem);
    // Critical section
    sem_post(&sem);
}

// Mutex
#include <pthread.h>
int main() {
    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
}

// Signal
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
int main() {
    pid_t pid = 97995;

    int result = kill(pid, SIGTERM);

    if (result == 0) {
        printf("Signal successfully sent!\n");
    } else {
        printf("Failed to send signal!\n");
    }
}