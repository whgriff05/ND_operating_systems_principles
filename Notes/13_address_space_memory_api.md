# Address Space, Translation, and Memory API

## Review from the first half

__OS Themes__
1. Virtualization (Process: CPU | Threads: Execution Context)
2. Concurrency (Synchronization: Race Conditions, Deadlock, etc | Mutexes, Condition Variables, Semaphores)
3. Persistence (File System)

## Virtual Memory

In a __batch__ system, there is a thin operating system and a single program.

In an __interactive__ system, there is an operating system and __many__ programs.

With the develoment of __multiprogramming__ and __time-sharing__, we needed a way to __protect__
programs from each other and to effectively __switch__ between them.

### Address Space

Recall the model of address space

```
- Stack: Local Variables and Function Parameters
    (grows downward)
    ...
    (grows upward)
- Heap: User Managed Data
- Data: Global Variables, Static Variables, String Literals
- Code: Program Instructions
```

### Challenges

__Protection:__ Prevent one process from interfering with another.

__Ownership:__ Each process believes it has sole control of all memory.
- Illusion of infinite memory

## Address Translation

Each address in our program is actually a __virtual address__ that must be translated
to a __physical address__. This virtualization allows us to __relocate__ a process to any
location in memory

__Goals__
1. __Transparency:__ Invisible to application
2. __Efficiency:__ As little overhead as possible
3. __Protection:__ Must isolate processes from each other

### Relocation

__Static__
- Any time you load a program into memory, rewrite all addresses
- Expensive upfront
- No actual protection
- Hard to relocate

__Dynamic__
- A better approach is to use special hardware (MMU) features to perform on-demand translation
- Protection provided
- Updatable
- Pay cost on-demand, not upfront

### Base and Bounds

The CPU has a __base register__ holding the starting point of physical memory, and a __bounds register__
holding the ending point in physical memory

The physical address is calculated as such
```
Physical Addr. = Base Register + Virtual Addr.
```

There are base and bounds for each process. Since each process has an execution context, and each 
EC has registers, __there exists base and bounds registers for each process__

### Example

Suppose we had an address space of 8 KB that has been loaded at physical address 32 KB.

(Also recall entirely in base-2: `K = 2^10`, `M = 2^20`, ...)

| Virtual Address | Physical Address |
| - | - |
| 0 KB |  32 KB |
| 1 KB |  33 KB |
| 5000 | ~37 KB |
| 9000 |  Fault |

```
Notes:
- 8 KB is the bounds
- 32 KB is the base
- When exceeding bounds, a fault is raised
```

__Pros of Base and Bounds:__ Protection, Relocation

__Cons of Base and Bounds:__ Hard to update, Prone to external fragmentation and internal fragmentation
