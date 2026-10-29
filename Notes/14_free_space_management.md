# Free Space Management

So far, all we've discussed is the __MMU's__ work. What does the OS do?

| Responsibility | Notes |
| - | - |
| Memory Accounting | Use a __data structure__ to track allocations (__free list__) |
| Hardware Management | Update the MMU (save and restore during a __context switch__) |
| Exception Handling | Register __exception handler__ and handle exceptions on __invalid access__ |

## Challenge: External Fragmentation

Because memory is divided into __blocks__, we may have non-contiguous free space

```
[ free ][ USED ][ free ]
0      10      20     30
```

There is not enough contiguous free space to reserve a space of 20
even though we have enough free space overall.

```
External Fragmentation = 1 - (Largest Free Block / All Free Space)
```

## Challenge: Internal Fragmentation

Suppose we over-allocate a specific block (for alignment, optimization, etc.)

```
[   free   ][   USED ][][   free   ]
0          10          20         30
```

We waste space due to unusable space in over-allocation.

```
Internal Fragmentation = All Unused Space / Heap Size
```

## Mitigating Fragmentation

### Splitting

Instead of always asking for more memory, we can __reuse__ a block
of free space.

```
[ USED ][     free     ]
0      10      20     30

// Locate a free block, split into 2

[ USED ][ USED ][ free ]
0      10      20     30
```

Helps reduce __internal fragmentation__

### Coalescing

When we free memory, we can check if there are contiguous free blocks and 
__coalesce__ or __merge__ them.

```
[ USED ][ USED ][ free ]
0      10      20     30

// Free, then merge consecutive free blocks

[ USED ][     free     ]
0      10      20     30
```

Helps reduce __external fragmentation__
