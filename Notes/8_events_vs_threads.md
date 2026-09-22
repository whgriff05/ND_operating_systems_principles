# Events vs Threads

*Recall: concurrency is just the structure of having __multiple streams of execution__, not necessarily running those streams at the same time*

## Events

If we only need to overlap I/O and computation, we can use __events__ to provide concurrency without parallelism

In this system, we use an __event loop__
- Register interest in events (callbacks)
- Event loop waits for event, then invokes corresponding handler
- Handlers are generally short-lived and not preempted

## Events vs Threads

| X       | Pros | Cons |
|---------|------|------|
| Events  | Efficient concurrency | Prone to denial-of-service, no parallelism |
| Threads | Parallelism | Complicated interface due to synchronization |

## Threads

We can implement __thread-based concurrency__ by using `pthreads` or POSIX threads, defining functions for
- Creating threads
- Waiting on threads
- Locking shared resources
- Notifying other threads

```
           [Worker] ---> (Exit)
             ^              |
             |              ⌄
[Main] --> (Create) ---> (Join) --> ...
```

### Comparison between Processes and Threads

| X | Process | Thread |
|-|-|-|
| Create New Task | `fork` + `exec` | `pthread_create`|
| Wait For Task | `wait` + `waitpid` | `pthread_join` |
| Lock Critical Section | `sigprocmask` (block signals) | `pthread_mutex_lock` + `pthread_mutex_unlock` |
| Notify Another Task | `sigaction` / `signal` | `pthread_cond_wait` + `pthread_cond_signal` | 

### Organization

__Many-to-One__
- Many user threads, one __kernel LWP__ (lightweight process)
- (+) Portable, flexible, straightforward to implement
- (-) No parallelism
- (-) If one thread blocks, they all block

__One-to-One__
- Each user thread has its own kernel LWP
- (+) Parallelism (we can use extra hardware resources)
- (+) One thread blocking does not block all threads
- (-) Limit to scaling (memory limit for the stack)
- (-) Limit to scaling (algorithm)

__Many-to-Many__ / (M:N)
- Many user threads all connected to many kernel LWPs
- (+) Scalability
- (+) Custom scheduling / flexibility
- (-) Really hard to implement correctly

*Note: while Many-to-Many seems like the best solution, Linux uses One-to-One for its overall thread architecture*

