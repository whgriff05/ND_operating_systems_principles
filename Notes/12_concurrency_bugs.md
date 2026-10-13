# Concurrency Bugs

## Bug: Atomicity Violation

__Example 1:__ \ 
A race condition occurs here \
Solution: both sides need locks

```py
# Thread 1
if buf_index + len < BUFSIZ:
    memcpy(buf[buf_index], log, len)

# Thread 2
buf_index += len
```

__Example 2:__ \
Overly aggressive with locking (there is a chance Thread 2 could run in between operations in Thread 1, leading to potential error) \
Solution: guard the __entire__ critical section

```py
# Thread 1
lock()
y = calculate()
unlock()

lock()
if (y == 0): y = 1
unlock()

# Thread 2
lock()
a = x / y
unlock()
```

## Bug: Order Violation

__Example 1:__ \
Potential error if Thread 2 is scheduled before Thread 1 (no queue exists) \
Solution: Use a __barrier__

```py
# Thread 1
mq.incoming = queue_create()

# Thread 2
if mq.incoming:
    data = queue_pop(mq.incoming)
```

__Example 2:__ \
Potential error if Thread 2 is scheduled before Thread 1 (queue would be deleted) \
Solution: Have some sort of `running` flag and `shutdown()` function

```py
# Thread 1
while True:
    queue_push(mq, request)

# Thread 2
queue_delete(mq.incoming)
```

## Bug: Deadlock

Occurs when each thread is waiting for another to give up a resource and none can run.

Usually, this is because there is a __cycle__ in the graph of dependencies.

## Bug: Livelock

Occurs when multiple threads are actively attempting to acquire resources, but 
no progress is made.

```py
# Thread 1
while True:
    Lock(l1)
    if Locked(l2):
        Unlock(l1)
        continue

    Lock(l2)
    ...
    Unlock(l2)
    Unlock(l1)

# Thread 2
while True:
    Lock(l2)
    if Locked(l1):
        unlock(l2)
        continue

    Lock(l1)
    ...
    Unlock(l1)
    Unlock(l2)
```

## Dining Philosophers

There are __5 philosophers__ eating dinner,
- There are only 5 forks
- To eat, a person needs 2 forks

This is __deadlock__ if everyone grabs their right or left fork

How do we break deadlock? Look at the __4 conditions:__
1. __Mutual Exclusion:__ Threads claim exclusive control of resources
    - Solution: Don't share; add more resources
2. __Hold and Wait:__ Threads hold resources while waiting for additional resources
    - Solution: Try, release, try, ...; could lead to __livelock__
3. __No Pre-emption:__ Resources cannot be forcibly removed from threads holding them
    - Solution: Have a run-time/scheduler that pre-empts threads and takes away resource
4. __Circular Wait:__ There exists a circular chain of threads such that each thread holds
one or more resources needed by another
    - Solution: Provide an order for acquiring shared resources
