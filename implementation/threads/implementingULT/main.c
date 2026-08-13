#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ucontext.h>
#include <sys/time.h>

#define MAX_THREADS 5
#define STACK_SIZE (12 * 1024)      // 12 KB
#define TIME_QUANTUM_US 200    // 2 ms

#define INVALID_THREAD ((size_t)-1)

typedef enum {
    READY,
    RUNNING,
    FINISHED
} State;

typedef struct TCB {
    size_t tid;
    State state;
    ucontext_t context;
    char stack[STACK_SIZE];
} TCB;

TCB threads[MAX_THREADS];

typedef struct Queue {
    size_t data[MAX_THREADS];
    size_t front;
    size_t rear;
    size_t count;
} Queue;

Queue readyQueue = {
    .front = 0,
    .rear = MAX_THREADS - 1,
    .count = 0
};

size_t current = INVALID_THREAD;
size_t totalThreads = 0;



void enqueue(size_t tid){
    if (readyQueue.count == MAX_THREADS)
        return;

    readyQueue.rear = (readyQueue.rear + 1) % MAX_THREADS;
    readyQueue.data[readyQueue.rear] = tid;
    readyQueue.count++;
}

size_t dequeue(){
    if (readyQueue.count == 0)
        return INVALID_THREAD;

    size_t tid = readyQueue.data[readyQueue.front];

    readyQueue.front = (readyQueue.front + 1) % MAX_THREADS;
    readyQueue.count--;

    return tid;
}


void createThread(void (*function)()){
    if (totalThreads >= MAX_THREADS) {
        fprintf(stderr, "Maximum threads reached!\n");
        exit(EXIT_FAILURE);
    }

    TCB *thread = &threads[totalThreads];

    thread->tid = totalThreads;
    thread->state = READY;

    getcontext(&thread->context);

    thread->context.uc_stack.ss_sp = thread->stack;
    thread->context.uc_stack.ss_size = STACK_SIZE;
    thread->context.uc_link = NULL;

    makecontext(&thread->context, function, 0);

    enqueue(totalThreads);

    totalThreads++;
}

void scheduler(int signum){
    (void)signum;

    size_t previous = current;

    if (previous != INVALID_THREAD &&
        threads[previous].state != FINISHED)
    {
        threads[previous].state = READY;
        enqueue(previous);
    }

    current = dequeue();

    if (current == INVALID_THREAD)
        return;

    threads[current].state = RUNNING;

    if (previous == INVALID_THREAD)
        setcontext(&threads[current].context);
    else
        swapcontext(&threads[previous].context,
                    &threads[current].context);
}


void startTimer(){
    struct sigaction sa;

    sa.sa_handler = scheduler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NODEFER;

    sigaction(SIGALRM, &sa, NULL);

    struct itimerval timer;

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = TIME_QUANTUM_US;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = TIME_QUANTUM_US;

    setitimer(ITIMER_REAL, &timer, NULL);
}

void thread1(){
    while (1) {
        printf("Thread 1\n");
        fflush(stdout);

        for (volatile long i = 0; i < 3000000; i++);
    }
}

void thread2(){
    while (1) {
        printf("Thread 2\n");
        fflush(stdout);

        for (volatile long i = 0; i < 3000000; i++);
    }
}

void thread3(){
    while (1) {
        printf("Thread 3\n");
        fflush(stdout);

        for (volatile long i = 0; i < 3000000; i++);
    }
}


int main(){
    createThread(thread1);
    createThread(thread2);
    createThread(thread3);

    startTimer();

    scheduler(0);

    return 0;
}