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

## Reader-Writer Locks

With semaphores, we can create interesting synchronization objects such as __reader-writer locks__

Once __readers__ acquire the writelock, __as many readers as possible__ can read data \
Once a __writer__ acquires the writelock, __no one else__ can access data

*Unfortunately, this can add overhead and is prone to __starvation__*

### Reader Preference

```py
size_t Readers    = 0
sem_t  WriterLock = 1
sem_t  ReaderLock = 1

Writer():
    sem_wait(WriterLock)
    write()
    sem_post(WriterLock)

Reader():
    # Are you the first reader? If so, get the writelock
    sem_wait(ReaderLock)
    if Readers == 0:
        sem_wait(WriterLock)
    Readers++
    sem_post(ReaderLock)

    read()

    # Are you the last reader? If so, give up the writelock
    sem_wait(ReaderLock)
    Readers--
    if Readers == 0:
        sem_post(WriterLock)
    sem_post(ReaderLock)
```

__Pro:__ Allow multiple readers

__Con:__ Writers can starve

### Fair

```py
size_t Readers    = 0
sem_t  WriterLock = 1
sem_t  ReaderLock = 1
sem_t  Queue      = 1

Writer():
    # There is a queue of both W and R threads to get the writelock
    # This ensures fairness
    sem_wait(Queue)
    sem_wait(WriterLock)
    sem_post(Queue)
    write()
    sem_post(WriterLock)

Reader():
    # Are you the first reader? If so, get the writelock
    sem_wait(Queue)
    sem_wait(ReaderLock)
    if Readers == 0:
        sem_wait(WriterLock)
    Readers++
    sem_post(Queue)
    sem_post(ReaderLock)

    read()

    # Are you the last reader? If so, give up the writelock
    sem_wait(ReaderLock)
    Readers--
    if Readers == 0:
        sem_post(WriterLock)
    sem_post(ReaderLock)

```

## Summary

Semaphores are good when you want __bounded access to a resource__ or a __barrier__

Unlike locks and condition variables, there is no __ownership__

An alternative to locks and condition variables
