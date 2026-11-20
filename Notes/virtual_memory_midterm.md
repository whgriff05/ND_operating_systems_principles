# Unit 03: Virtualization with Memory

## Intro to Virtual Memory

Need for virtual memory: we need ways to protect programs from each other and to effectively switch between them

Recall Address Space:
```
- Stack: Local Variables and Function Parameters
    (grows downward)
    ...
    (grows upward)
- Heap: User Managed Data
- Data: Global Variables, Static Variables, String Literals
- Code: Program Instructions
```

### Address Translation

Each address in our program is actually a __virtual address__ that must be translated to a __physical address__. Allows us to __relocate__ a process to anywhere in memory.

We prefer dynamic (on-demand, __MMU__) relocation over static (rewriting on program-load) relocation since it is quicker/more efficient (uses hardware)

## Method 1: Base and Bounds

The CPU has a __base register__ holding the starting point of physical memory, and a __bounds register__ holding the ending point in physical memory

```
Phys. Addr. = Base Reg. + Virt. Addr.
```

These registers are stored in execution context, therefore, there are Base and Bounds registers for each process

## Common Memory Errors

- __Segmentation Fault:__ No memory is allocated for a use
- __Buffer Overflow:__ Not enough memory was allocated for a use
- __Uninitialized Read:__ Allocated memory was not initialized (given a known value)
- __Memory Leak:__ Memory was not freed
- __Dangling Pointer:__ Pointer to unallocated memory
- __Use-after-free:__ Use of a dangling pointer
- __Double Free:__ Pointer is freed twice (leads to undefined behavior)
- __Invalid Free:__ Attempt to free non-dynamically allocated memory

## Fragmentation

When we have non-contiguous free space

__External:__ 1 - (Largest Free Block / All Free Space)
- Non-contiguous free space between blocks
- Ratio gives you an idea of how densely populated the free space is

__Internal:__ All Unused Space / Heap Size
- Unusable space in over-allocation (usually due to alignment)

Two main ways we mitigate this: __splitting__ and __coalescing__

## Free List

We create a linked list of `Block` structs containing the following header information about a block of memory
- __size:__ number of bytes used
- __capacity:__ number of bytes allocated (aligned)
- __previous:__ pointer to previous block
- __next:__ pointer to next block

## Method 2: Segmentation

Each component of the address space is considered a separate __segment__, and therefore gets its own set of base and bounds registers

This allows for __code sharing__ (multiple processes can share common code sections) but brings the potential issue of a __growth direction__ where the stack grows in a different direction, and keeping track of stack-heap intersection

Need 2 segment bits (for 4 segments)

Main issue is external fragmentation, where the variable-sized segments lead to many small holes

## Method 3: Paging

Each process has an address space consisting of fixed-size virtual __pages__ mapping to physical __page frames__ (of the same size)
- *typical page size is 4KB*

Each process has a __page table__ storing this page --> page frame mapping

### Issues

1. Overhead: each process needs a page table (which can be quite large)
2. Indirection: page table entries are stored in physical memory too, so each memory lookup requires 2 memory accesses (pull PTE, pull data)

### Cache (Translation-Lookaside Buffer)

TLB caches memory accesses to solve Issue 2 of paging

On a context switch, OS either flushes TLB or sets the address space identifier register to the current PID (so MMU knows which entries are valid)

On misses
- If handled by hardware, MMU walks page table, finds PTE, and updates TLB itself (x86 and ARM)
- If handled by software, MMU raises fault, software looks up PTE and updates TLB (SPARC and MIPS)

### Multi-Level Page Tables

Having a tree-based page table helps solve Issue 1 by breaking them down

Condense every X PTEs into its own page (PDE)

This page table of page tables is called a Page Directory
