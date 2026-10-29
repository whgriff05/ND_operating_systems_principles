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

## Memory API

```C
// How would we allocate an array of 1,000,000 ints?
int* a = malloc(1000000 * sizeof(int));
int* b = calloc(1000000, sizeof(int));

// How do we deallocate these arrays?
free(a);
free(b);
```

### Common Errors

```C
// Issue
char* buffer;
fgets(buffer, BUFSIZ, stdin);
```

__Segmentation Fault:__ We do not allocate __any__ memory for this to use

```C
// Solution
char buffer[BUFSIZ];
fgets(buffer, BUFSIZ, stdin);
```

---

```C
// Issue
char* src = "abcdef";
char* dst = malloc(strlen(src));

strcpy(dst, src);
```

__Buffer Overflow:__ Did not allocate __enough__ memory

```C
// Solution
char* src = "abcdef";
char* dst = strdup(src);
```

---

```C
// Issue
char* s = malloc(100 * sizeof(char));
puts(s);
```

__Uninitialized Read:__ Did not initialize memory

```C
// Solution
char* s = calloc(100, sizeof(char));
puts(s);
```

---

```C
// Issue
for (int i = 1; i < argc; ++i) {
    char* s = strdup(argv[i]);
    process(s);
}
```

__Memory Leak:__ Did not free memory

```C
// Solution
for (int i = 1; i < argc, ++i) {
    char *s = strdup(argv[i]);
    process(s);
    free(s);
}
```

---

```C
// Issue
for (int i = 1; i < argc; ++i) {
    char* s = strdup(argv[i]);
    process(s);
    free(s);
    puts(s);
}
```

__Dangling Pointer:__ Pointer with no allocated memory \
__Use-After-Free:__ Using a pointer with nothing

---

```C
// Issue
for (int i = 1; i < argc; i++) {
    char *s = strdup(argv[i]);
    if (check(s)) { free(s); }
    free(s);
}
```

__Double Free:__ Pointer is freed twice

---

```C
// Issue
for (int i = 1; i < argc; i++) {
    char *s = argv[i];
    process(s);
    free(s);
}
```

__Invalid Free:__ Attempt to free non-dynamically allocated memory

### Multiple Levels of Memory Management

Process Level: individual `malloc`s and `free`s

OS Level: deallocates an entire process when it completes (including __all__ allocated memory)

You do not have to *always* clean up after yourself, __but for longer running processes, it is typically
better to handle it yourself to avoid memory leaks__

