# Event-based Concurrency

## Motivation: `counter`

Create an application, `counter`, that does the following
- periodically increment ac ounter
- provide a shell prompt that allows the user to enter in a command
    - `count`: returns current value of counter
    - `reset`: resets counter to 0
    - `exit`: quit program

## Concurrency

A program is __concurrent__ when we try to structure it such that we have __multiple streams of execution__

*Concurrency is about the __structure__ of the program, not the resources*

Whenever we have the __hardware resources__ to simultaneously execute __multiple streams__, we have __parallelism__

### Internal

Concurrent tasks in `counter`:
1. Increment counter
2. Handle shell input

In addition to *inter-process concurrency*, we can have internal concurrency in a single process:
1. Foreground vs Background Thread
2. Mix I/O and Computation
3. Signals

### Threads

We can implement __internal concurrency__ by dividing a process into __multiple streams of execution__

Recall machine state: __address space, kernel state, and execution context__. What is shared between these streams?
- Address space is __kinda shared__
    - Threads have __different, isolated__ stacks
    - Threads share code, data, and heap
- Kernel state is __shared__
- Execution context is __not shared__, they need isolated execution!
    - Each thread has its own set of registers
    - Each thread has its own stack pointer (note the isolated stacks under the address space)

### Interview Question

__Is this shared between threads running `f()`?__ \
No, this is a variable on the stack

```C
int f() {
    int i;
    ...
}
```

__Is this shared between threads running `g()`?__ \
It depends, what is `s` and what does it point to?

```C
int g(char* s) {
    *s = _____;
    ...
}
```

### Sharing Problem

Shared resources are an issue - the regions of code where this happens are called the __critical sections__

Shared resources become an issue with uncontrolled scheduling, and the lack of order of execution can lead to __race conditions__

We need __atomicity__: an uninterruptable operation that either fully completes or does nothing; we will accomplish this with __mutual exclusion__


