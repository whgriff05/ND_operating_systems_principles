# System Calls

Libraries use system calls to interface with the kernel


Recall the __system call__ from Systems Programming: when a user application requests a service/resource from the operating system kernel
- I/O: `open`, `close`, `read`, `write`, `lseek`
- File: `stat`, `chmod`, `chown`, `unlink`
- Directory: `opendir`, `readdir`, `closedir`
    - *Note: directory ones are high-level functions since Linux recommends using the higher level ones*

To track an application's system calls, use the `strace` command
- *Note: `strace` prints to __stderr__, so if needing to pipe, use '|&`*

In the terms of the processor, the system call is __just a number__ that we put in register a, followed by the arguments it needs, then call a __trap__ interrupt
- A __trap__ is a forced interrupt for the CPU to stop processing the program's instructions and move to processing the operating system

## Process Address Space (Review)

```
Stack: parameters, local variables
|
⌄

^
|
Heap: user-managed memory
Data: string literals, static and global variables
Code: program instructions
```

## User vs Kernel Mode

An __interrupt__, __trap__, or __exception__ is an event that forces the processor to transition from __user mode__ to __kernel mode__
- *Note: on Intel machines, user mode is Ring 3, kernel mode is Ring 0*

User Mode has restricted hardware access, Kernel Mode has unrestricted hardware access

## Processor Events

Types of events and exceptions:
- __interrupt__: signal from I/O device
    - asynchronous
    - always returns to next instruction
- __trap__: intentional exception
    - synchronous
    - always returns to next instruction
- __fault__: potentially recoverable error
    - synchronous
    - might return to next instruction
- __abort__: nonrecoverable error
    - synchronous
    - never returns to next instruction

When these events happen, the processor consults the __interrupt vector table__ or __trap table__ to determine what to do next


### How a system call works

1. Upon initialization, the kernel registers handlers in the trap table (which registers function pointers to execute based on the syscall number)
2. User process makes a system call
    a. Records the system call number
    b. Records the system call arguments
    c. Triggers a trap
3. Processor uses the trap table to select the handler
4. Kernel performs handler

