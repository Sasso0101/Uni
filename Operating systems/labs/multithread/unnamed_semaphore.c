#include <semaphore.h>

int main() {
    sem_t *sem;
    sem = sem_open("SEM", O_CREAT, 0666, 1);
    sem_wait(&sem);
    // Critical section
    sem_post(&sem);
}