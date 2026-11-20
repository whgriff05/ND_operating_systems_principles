# Swapping

Recall that the OS is lazy:
- When `malloc` (or adjacent) is called, only __virtual__ memory is reserved
- OS does not reserve physical memory unless you actually request to use it

## Memory Hierarchy

```
(Fast / Expensive)
1. Registers
2. Cache
3. RAM
4. Disk (SSD, HDD, ...)
5. Cloud
(Slow / Cheap)
```

## Swapping: Overview

The __swap space__ is a portion of a disk reserved for moving pages back and forth
- Used to hold pages of currently running processes
- The __present bit__ in a PTE tracks whether or not a page is in physical memory 

## Page Replacement

Ideally, we would minimize the amount of misses: we want to __evict pages__ that will be accessed furthest into the future \
Unfortunately, this is difficult to determine so we consider different replacement policies
- FIFO (poor): ignoring access patterns (temporal locality)
- Random (so-so): averages out in performance
- LRU (best): best choice BUT expensive due to linear scan and accounting

## The Clock Algorithm

- OS arranges pages in a circular list
- A clock hand points to a current page
- When replacement occurs, OS checks used bit of the current page
    1. If 0: evict page
    2. Else: clear used bit, move to next page

## Page Selection

Two strategies on bringing pages back into physical memory

1. __Demand Paging:__ bringing pages back into physical memory when they are accessed
2. __Prefetching:__ bringing pages back into physical memory ahead of time

## Danger

Suppose the amount of memory requested by running processes (i.e. working set) exceeds the available physical memory...

The constant page eviction and swapping is called __thrashing__ (memory's livelock)

Linux's solution is the OOM-Killer

## Laziness in Swapping


