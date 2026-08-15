#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

/*
 * Test-and-Set Spinlock
 *
 * lock = false -> lock is free
 * lock = true  -> lock is held
 */
bool lock = false;

/*
 * Atomically:
 *   1. Read the current value of lock
 *   2. Set lock = true
 *   3. Return the old value
 *
 * If the old value was false, we acquired the lock.
 * If it was true, another thread already owns it.
 */
void acquire_lock()
{
    while (__atomic_test_and_set(&lock, __ATOMIC_SEQ_CST)) {
        // Busy waiting (spinning)
    }
}

/*
 * Atomically release the lock.
 */
void release_lock()
{
    __atomic_clear(&lock, __ATOMIC_SEQ_CST);
}

void* process(void* arg)
{
    acquire_lock();

    /*
     * Critical Section
     *
     * Only one thread can execute this section
     * at a time.
     */
    printf("Process %ld is inside critical section\n", (long)arg);

    release_lock();

    return NULL;
}

int main()
{
    pthread_t p1, p2;

    // Create two competing threads
    pthread_create(&p1, NULL, process, (void*)1);
    pthread_create(&p2, NULL, process, (void*)2);

    // Wait for both threads to finish
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    return 0;
}