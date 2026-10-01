# Thread Condition Variables

```py
# Attempt 1: No locks
# Issues: Race conditions, no bounds checking

def Producer(queue):
    while True:
        data = produce()
        queue.push(data)

def Consumer(queue):
    while True:
        data = queue.pop()
        consume(data)
```

```py
# Attempt 2: No locks, if check
# Issues: Only locks a certain part of the critical section

def Producer(queue):
    while True:
        data = produce()
        if not queue.full():
            queue.push(data)

def Consumer(queue):
    while True:
        if not queue.empty():
            data = queue.pop()
            consume(data)
```

```py
# Attempt 3: No locks, while check
# Issues: Only locks a certain part of the critical section

def Producer(queue):
    while True:
        data = produce()
        while queue.full():
            continue

        queue.push(data)

def Consumer(queue):
    while True:
        while queue.empty():
            continue

        data = queue.pop()

        consume(data)
```

```py
# Attempt 4: Locks
# Issues: Only locks a certain part of the critical section

def Producer(queue):
    while True:
        data = produce()
        while queue.full():
            continue

        lock()
        queue.push(data)
        unlock()

def Consumer(queue):
    while True:
        while queue.empty():
            continue

        lock()
        data = queue.pop()
        unlock()

        consume(data)
```

```py
# Attempt 5: Locks
# Issues: Deadlock in busywaiting

def Producer(queue):
    while True:
        data = produce()
        lock()
        while queue.full():
            continue

        queue.push(data)
        unlock()

def Consumer(queue):
    while True:
        lock()
        while queue.empty():
            continue

        data = queue.pop()
        unlock()

        consume(data)
```

## Condition Variables: Monitor

We use __shared condition variables__ to avoid __deadlock__. We have locks around our critical section to make it thread-safe, but we use a condition variable to ensure that a thread does not hog the lock forever.

We also abstract this so that people using the concurrent data structure do not have to worry about when to lock and unlock.

```py
def push(queue, data):
    lock()

    while queue.full():
        cond_wait()
        
    queue.push(data)
    unlock()

def pop(queue):
    lock()

    while queue.empty():
        cond_wait()

    data = queue.pop()
    unlock()
    return data
```

`pthread_cond_wait(&cond, &lock)`:
1. Give up lock
2. Go to sleep
3. Upon signal from `cond`, wake up
4. Take back lock
- *We want to do this in a while loop because the next time the thread wakes up, the condition might __not__ have changed*

## Separate Condition Variables

Sometimes it is good practice to separate signals for producers and consumers. This way, the
scheduler doesn't only schedule producers or only schedule consumers, causing no action to
take place
