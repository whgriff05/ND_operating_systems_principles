# Unit 02: Concurrency with Threads

## What is Concurrency

__Concurrency__ is when a program is *structured* with __multiple streams of execution__

__Parallelism__ is when there are *hardware resources* to support *simultaneous execution*

## Threads
A thread is a stream of execution; an __abstraction of an execution context__
- Shared: Address Space (code, data, heap), Kernel State
- Not Shared: Address Space (stack), Execution Context
- Areas where shared resources between threads are accessed are __critical sections__; lack of order of execution in critical sections leads to __race conditions__
- We need __atomicity:__ an uninterruptable operation that either fully completes or does nothing (accomplished with __mutual exclusion__)

```
           [Worker] ---> (Exit)
             ^              |
             |              ⌄
[Main] --> (Create) ---> (Join) --> ...
```

### Organization of Thread Implementations

__Many-to-One__
- Many user threads map to one __kernel lightweight process__
- (+) Portable, flexible, straightforward to implement
- (-) No parallelism
- (-) If one thread blocks, they all block
- (-) Scheduler only sees one process

__One-to-One__
- Each user thread has its own kernel LWP
- (+) Parallelism
- (+) One blocking thread does not block all threads
- (-) Limit to scaling (memory limit for the stack)
- (-) Limit to scaling (algorithm... Linear search in `queue_remove`)

__Many-to-Many__ / __(M:N)__
- (+) Scalability
- (+) Custom scheduling / flexibility
- (-) *Really* hard to implement correctly

*Note: Linux uses One-to-One*

### Thread Pool

When a certain number of pre-created threads are fed a queue of tasks to perform
- Allows throttling since only a certain number of tasks can be processed at a time

## Events vs Threads

__Events__ can provide concurrency w/o parallelism if we only need to overlap I/O and computation
- Register interest in events (callbacks)
- Wait for an event, invoke corresponding handler
- Handlers are generally short-lived and not pre-empted

Events are prone to denial of service

*Note: to help stop I/O blocking when no I/O is occurring, use `select()/poll()` system calls*

## Thread Locking

Race conditions occur because each line of C code is rarely just one assembly instruction, and when they overlap, they can try to edit the same memory, causing a disjoint of what the value at that memory truly is

To solve this, we use a __lock__ or __mutual exclusion (mutex)__ to guard a __critical section__

### Types of Lock
1. Interrupt Disabling
    - (+) No interrupt can cause the thread to stop or the lock to be removed
    - (-) Can possibly lose interrupts
    - (-) Needs to trust threads to not hog processing
    - (-) Doesn't scale to multiple processors (disables interrupts for whole system)
2. Spin Lock: Constantly check availability of lock
    - (-) Race condition possible in getting the lock's availability
    - (-) Busy waiting
3. Test and Set: Use a special __atomic__ hardware instruction to gain availability
    - (-) Busy waiting
4. Yielding: Instead of busy waiting, yield the CPU as we spin
    - (-) Starvation
    - (-) Lots of context switches

## Condition Variables

We use __shared condition variables__ to avoid __deadlock__ (when a thread is stuck waiting for a condition to be met while holding a lock). Typically, we try to abstract this into a concurrent data structure (__monitor__) so users do not have to worry about when to unlock and lock.

`pthread_cond_wait(&cond, &lock)`
1. Give up lock
2. Put thread to sleep
3. Upon signal from `cond`, wake back up
4. Take back lock
- *We do this in a while loop because the next time the thread wakes up, the condition __might not have changed__*

*Note: sometimes, it is good practice to separate signals for producers and consumers so the scheduler schedules threads of both types*

## Semaphores

A __semaphore__ is a synchronization primative that consists of an integer we manipulate with two operations

```py
def sem_wait(sem_t* sem):
    sem->count -= 1
    if (sem->count < 0):
        sem.wait() # Put thread to sleep

def sem_post(sem_t* sem):
    sem->count += 1
    sem->threads.pop(0) # Wake up next waiting thread in internal queue
```

### Locks and Conds with Semaphores

```C
sem_t lock;
sem_init(&lock, 0, 1); // Initial value of 1 (you only want 1 thread to run a critical section at a time)

sem_wait(&lock);
operation();
sem_post(&lock);
```

```C
sem_t cond;
sem_init(&cond, 0, 0); // Initial value of 0 (Instant sleep/signal)

/* Thread 1 */
sem_wait(&cond); // Wait to wait on the condition to be met
operation();

/* Thread 2 */
operation();
sem_post(&cond); // Post to signal the condition
```

*Note: semaphores do __not__ need loops (they track state internally and handle sleeping themselves); putting a semaphore in a lock can cause deadlock*

*Note: if using semaphores as __both__ locks and condition variables, reverse the order of implementation to not go to sleep holding the lock*

*Note: a semaphore might have a different count if used as a __barrier__ (causes operation when a numerical condition is met)*

## Reader Writer Locks
Type of synchronization object typically made with semaphores
- Once readers acquire the writelock, __as many readers as possible__ can read data
- Once a writer acquires the writelock, __only the singular writer__ can access data

For fairness: have both a writelock and a readlock so that readers don't starve out the writer

## Concurrency Bugs

### Atomicity Violation

A critical section operation happens with no lock/mutual exclusion \
Overly aggressive with locking causes gaps of no mutual exclusion between coinciding operations

### Order Violation

Potential errors occur if dependent thread operations are scheduled out of order

### Deadlock

A cycle of dependencies causes threads to wait for others to give up resources; no one can run

__4 conditions of deadlock__
1. __Mutual Exclusion:__ Threads claim exclusive control of resources
    - Solution: Don't share; add more resources
2. __Hold and Wait:__ Threads hold resources while waiting for additional resources
    - Solution: Try, release, try...; unfortunately could lead to __livelock__
3. __No Pre-emption:__ Resources cannot be forcibly removed from threads holding them
    - Solution: Have a run-time scheduler that that pre-empts threads and takes away resource
4. __Circular Wait:__ There exists a circular chain of threads such that each thread holds one or more resources needed by each other
    - Solution: Provide an order for acquiring shared resources

### Livelock

Multiple threads are actively attempting to acquire resources, but no progress is made

## Summary Table from Review Lecture

| Synchronization Primative | Behavior | Problems they Prevent |
|-|-|-|
| Locks | Ownership | Race Condition |
| Condition Variables | Ownership | Busy Waiting + Deadlock (sometimes) |
| Semaphores | Signalling | Race Conditions, Busy Waiting, Deadlock (sometimes) |

## Reading 04

```C
void* sha1sum_thread(void* arg) {
    char* path = (char *)arg;
    char cksum[SHA_DIGEST_LENGTH * 2 + 1] = {0};

    if (!sha1sum_file(path, cksum)) {
     return (void*)1;
    } else {
     printf("%s  %s\n", cksum, path);
    }
    return (void*)0;
}

int main() {
    pthread_t thread[argc-1];
    for (int i = 1; i < argc; i++) {
        pthread_create(&(thread[i-1]), NULL, sha1sum_thread, (void*)(argv[i]));
    }

    for (int i = 0; i < argc-1; i++) {
        void* rv;
        pthread_join(thread[i], &rv);
        count += (int)(long)rv;
    }
}
```

## Reading 05

```C
void* sha1sum_thread(void* arg) {
    Queue* q = (Queue*)arg;
    size_t failures = 0;
    char cksum[SHA_DIGEST_LENGTH * 2 + 1] = {0};
    char* path = NULL;

    while ((path = queue_pop(q))) {
        if (!sha1sum_file(path, cksum)) {
            return (void*)1;
        } else {
            printf("%s  %s\n", cksum, path);
        }
    }

    return (void*)failures;
}

int main() {
    Queue* q = queue_create(SENTINEL, QSIZE);
    Thread threads[MAX_THREADS];
    int count = 0;

    for (int i = 0; i < MAX_THREADS; ++i) {
        thread_create(&threads[i], NULL, sha1sum_thread, (void*)q);
    }

    for (int i = 1; i < argc; ++i) {
        queue_push(q, argv[i]);
    }

    for (int i = 0; i < MAX_THREADS; ++i) {
        queue_push(q, SENTINEL);
    }

    size_t individual_failures = 0;
    for (int i = 0; i < MAX_THREADS; ++i) {
        thread_join(threads[i], (void**)&individual_failures);
        count += individual_failures;
    }

    queue_delete(q);
}
```

## Reading 06

```C
void* sha1sum_thread(void* arg) {
    sem_wait(&Running);

    char* path = (char *)arg;
    char cksum[SHA_DIGEST_LENGTH * 2 + 1] = {0};

    if (!sha1sum_file(path, cksum)) {
        sem_wait(&Lock);
        Failures++;
        sem_post(&Lock);
    } else {
        printf("%s  %s\n", cksum, path);
    }

    sem_post(&Running);

    return (void*)0;
}

int main() {
    sem_init(&Lock, 0, MAX_THREADS);
    sem_init(&Running, 0, MAX_THREADS);

    pthread_t thread[argc-1];
    for (int i = 1; i < argc; i++) {
        pthread_create(&(thread[i-1]), NULL, sha1sum_thread, (void*)(argv[i]));
    }

    for (int i = 0; i < argc-1; i++) {
        pthread_join(thread[i], NULL);
    }

    return Failures;
}
```

## Project 02 Notes

### Chat Application

- Main thread: drove the input and the terminal handling
- SMQ receiver thread: constantly checked the queue for new messages, populating the chat window

### SMQ

- `smq_puller` pulled messages from the server, placing them in the __incoming__ queue
- `smq_pusher` pushed messages to the server, taking them from the __outgoing__ queue

### Possible to implement w/o threads?

Yes, but it would __not work well__

Use events with `select()/poll()` and have a shorter timeout on requests, size checks on queues
