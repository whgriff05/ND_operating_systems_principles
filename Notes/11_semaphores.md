# Semaphores

A __semaphore__ is a synchronization primative that consists of an integer we can manipulate with two operations:
- `sem_wait()`: decrease value; if value becomes negative, make the thread wait
- `sem_post()`: increase value; wake up the next waiting thread

Internally, a semaphore has a queue of threads

## Lock with Semaphores

```C
sem_t m;
sem_init(&m, 0, 1); // (sem_t*, <is shared b/t processes>, initial value)
// Init to 1 because you want only 1 thread to run at a time

sem_wait(&m); // Wait to acquire the lock
operation();
sem_post(&m); // Post to release the lock
```

## Condition Variable with Semaphores

```C
sem_t m;
sem_init(&m, 0, 0);
// Init to 0

/* Thread 1 */
sem_wait(&m); // Wait to wait on the condition
operation();

/* Thread 2 */
operation();
sem_post(&m); // Post to signal the condition
```

## More

Condition variables need loops to handle spurious interrupts \
Semaphores do __not__ need loops: they track state internally and are kept asleep until a post occurs \ 
Putting a semaphore in a loop can cause deadlock

Also, if using semaphores as locks/condition variables, the order is reversed \
This is because you do not want to sleep holding the lock -> cause of __deadlock__

Also, watch for initial value things \
A producer should not be blocked until structure is full \
A consumer should not be blocked until structure is empty

