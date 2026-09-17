# Unit 01: Virtualization with Processes

## Beginnings

Recall the __processor__ with an __ALU__ performing arithmetic operations and a __Controller__ controlling ALU's I/O and operations. The processor connects to __RAM__ and (via RAM), __disk (storage)__ and I/O devices

Recall the 5 step boot sequence:
1. __BIOS/UEFI__ checks computer usability (I/O existence, connected RAM) and finds the __MBR/GPT__ to see how the storage disk is partitioned
2. __MBR/GPT__ partition scheme helps locate the __bootloader__
3. __Bootloader__ prepares the computer to load the kernel
4. __Kernel__
5. __Init System__ opens applications and services, is the overall father of all processes and the orphanage of deserted processes

## Taxonomy, History, and Themes

Purpose-built operating systems
- __Mainframe__ processes batch instances of a single job or task
- __Server__ provides a service to multiple users
- __Personal__ supports a single user with many applications
- __Embedded__ runs on resource-constrained devices running a limited amount of programs
- __Real-Time__ provides consistent guaranteed actions

__Monolithic Kernels__ are a single large program running entirely in kernel mode, whereas __microkernels__ consist of multiple programs working together with the help of a *privileged intermediary*

We want multiprogramming (the ability to run multiple different actions): multiple tasks running concurrently (multiprogramming), one task cannot manipulate data of another (memory protection), and tasks splitting process time among users (timesharing)

An operating system is a __body of software__ that enables other programs to interact with each other and the __physical hardware__ in an efficient matter with __Virtualization__, __Concurrency__, and __Persistence__.

## System Calls

The way programs and libraries interface with the kernel

To the processor, a system call is just a __number__ that we put in a register along with its arguments. When a __trap interrupt__ is called, the CPU will stop processing the program and move the processing to the operating system by consulting the __trap table__

An __interrupt__, __trap__, or __exception__ is an event that forces the processor to transition from __user mode__ to __kernel mode__ to gain unrestricted hardware access

### Processor Events
- __interrupt__: signal from I/O device
    - asynchronous; always returns to next instruction
- __trap__: intentional exception
    - synchronous; always returns to next instruction
- __fault__: potentially recoverable error
    - synchronous; might return to next instruction
- __abort__: nonrecoverable error
    - synchronous; never returns to next instruction

Processor consults the __interrupt vector table__ (or __trap table__) to determine what to do next

## Processes

Recall that a __process__ is a __loaded instance of a program__ (a __unit of allocation__) with a __machine state__
- __addres space__: code, data, heap, stack
- __kernel state__: PID, owner, file descriptors
- __execution context__: program counter, registers, stack pointer

The operating system virtualizes the CPU so that each process gets a certain time slice, or share of the processor's time. This can happen through __cooperative multitasking__ where the OS trusts the processes to be fair and voluntairly yield the processor, or __preemptive multitasking__ where the OS sets a __timer interrupt__ to periodically pause the running process and possibly switch to another

### Context Switch

1. (Processor): Timer interrupt triggers, processor moves to kernel mode, jumps to the trap handler as defined in trap table
2. (Kernel): Determines which process to run next, saves current registers, loads next registers
3. (Processor): Switches processes, moves back to user mode, jumps to program counter of selected process

### Process Lifespan States

```
        |-> Ready -----|
        |   ⌄ ^ ^      |
New ----|-> Running ---|---> Terminated
        |   ⌄   |      |
        |-> Blocked ---|
```

- Ready -> Running:   Interrupt, then scheduled
- Running -> Blocked: Start a system call
- Blocked -> Ready:   System call is completed

Recall the Process Life Cycle

```

Parent --- fork() ---> Child
    |                   |
    |                   exec()
    |                   |
wait() <--------------- exit()
    |
    ⌄

```

`fork()` allocates a new child process with a __copied machine state__

`exec()` clears address space, loads new code, and resets execution context

`exit()` exits process and reports exit status to parent

`wait()` receives child exit status and deallocates process

## Scheduling

We want to determine which process runs next on a __timer interrupt__, __process termination__, or __system call__

__Turnaround Time__: time between a job arriving on the schedule queue and the job being completed
- `T_turnaround = T_finish - T_arrival`
- *throughput / volume*

__Response Time__: time between a job arriving on the schedule queue and the job starting
- `T_response = T_start - T_arrival`
- *latency / delay*

### FIFO

- Straightforward algorithm: execute jobs in the order they arrive
- Good turnaround time (generally), but poor response time
- Can suffer from the __convoy effect__ where a long job can hog the processor time before any other process starts
- Sometimes benefits from the *Shortest Job First* heuristic

### Round-Robin

- Straightfoward algorithm: execute jobs for a specific amount of time, cycling so each gets the same amount of time
- Poor turnaround time, but very strong response time
- Requires a timer interrupt (pre-emption)
- Overhead due to constant context switching (time consuming)
- Divides the CPU evenly, making this a __fair__ algorithm

### MLFQ

- Algorithm:
    - Within the highest occupied level, run the jobs in round-robin
    - New jobs start at highest priority
    - Lower priority of a job once its time allotment expires
    - After a period of time, all jobs move to top priority
- Prioritizes __new, short, I/O heavy jobs__ over __old, long, compute-heavy jobs__ 
- Devolves into round-robin
- Involves some tricks and magic (voodoo) numbers
- Without a priorty boost to processes, processes can __starve__ at the lowest priority where higher priority processes will always get to run over them

## Additions

### `pqsh`

Other notable system calls
- `setitimer()` - sets an interval timer that sets off `SIGALRM` after time
- `sigaction()` - to handle signals when they are received

### Round-Robin Scheduling Rule

Add a new process first, then rotate the queue
