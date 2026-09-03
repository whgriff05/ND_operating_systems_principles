# Processes

## An overview from Systems Programming

A __process__ is a __loaded instance of a program__; it is a unit of __allocation__ (*resources, privileges, etc.*)
- __address space__: code, data, heap, stack
- __kernel state__: PID, owner, file descriptors
- __execution context__: program counter, registers, stack pointer

The combination of a process's address space, kernel state, and execution context is called its __machine state__

## Time-Sharing

The operating system __virtualizes__ the __processor (CPU)__ via the process abstraction

Each task is associated with a process, which gets a certain __time slice__, or share of the __processor time__

## Multitasking

__Cooperative__ Multitasking:
- The operating system __trusts__ the processes to be fair and voluntarily __yield__ the processor

__Preemptive__ Multitasking:
- The operating system sets a __timer interrupt__ to periodically __pause__ the running process and __possibly switch__ to another

## Context Switch

When a timer interrupt goes off, the kernel has the option of performing a __context switch__ (i.e. *switch from one process to another*)

1. (Processor): Timer interrupt triggers, move to kernel mode, jump to trap handler
2. (Kernel): Determine which process to run next, save current registers, load next registers
3. (Processor): Switch processes, move to user mode, jump to program counter of selected process

## Process Lifespan States

```
        |-> Ready -----|
        |   ⌄   ^      |
New ----|-> Running ---|---> Terminated
        |   ⌄   ^      |
        |-> Blocked ---|
```

- Ready -> Running:    Interrupt, then scheduled
- Running -> Blocked:  Start a system call
- Blocked -> Ready:    System call is completed

## Process Life Cycle

1. A current process __A__ calls `fork()`, allocating a new process __B__ with a copied machine state
2. New process __B__ calls `exec()` to clear the address space, load the new code, and reset the execution context
3. Process __B__ exits with `exit()`, reporting exit status to parent
4. Process __A__ `wait()`s, receiving that child's exit status and cleaning it up (deallocated)
