#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

sem_t sem;

void *process1(void *arg)
{
    printf("P1-A\n");
    sleep(1);

    printf("P1-B\n");

    // Allow P2-C to execute
    sem_post(&sem);

    sleep(2);

    printf("P1-C\n");
    printf("P1-D\n");

    return NULL;
}

void *process2(void *arg)
{
    printf("P2-A\n");
    sleep(1);

    printf("P2-B\n");

    // Cannot continue until P1-B is finished
    sem_wait(&sem);

    printf("P2-C\n");
    printf("P2-D\n");

    return NULL;
}

int main()
{
    pthread_t p1, p2;

    // Binary semaphore initially 0
    sem_init(&sem, 0, 0);

    pthread_create(&p1, NULL, process1, NULL);
    pthread_create(&p2, NULL, process2, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    sem_destroy(&sem);

    return 0;
}