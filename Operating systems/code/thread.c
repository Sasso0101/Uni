#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct params {
    int min;
    int max;
    int ret;
};

void *runner(void *param); /* the thread */

int main(int argc, char *argv[])
{
    pthread_t tid[10]; /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */

    /* get the default attributes */
    pthread_attr_init(&attr);

    int sum = 0;

    /* create the thread */
    for (int i = 0; i < 10; i++) {
        struct params* p = malloc(sizeof(struct params));
        p->min = i*10;
        p->max = i*10+10;
        p->ret = 0;
        pthread_create(&tid[i],&attr,runner,p);
    }
    for (int i = 0; i < 10; i++) {
        void *pointer;
        pthread_join(tid[i], pointer);
        sum += *(int*)pointer;
    }
    printf("%d", sum);
}

/**
 * The thread will begin control in this function
 */
void *runner(void *param) 
{
    struct params* param_p = (struct params*) param;
    // int i, upper = atoi(param);
    int sum; /* this data is shared by the thread(s) */

	for (int i = param_p->min; i <= param_p->max; i++)
		sum += i;
    param_p->ret = sum;
	pthread_exit(&(param_p->ret));
}
