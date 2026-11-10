# Segmentation

## Our current lies

1. Infinite memory
2. Contiguous allocation
3. Dedicated ownership

To perform this lie, we need __address translation__ \
We rely on the MMU to do it on-demand in hardware

Currently, we have discussed the __base and bounds__ technique \
Issues:
- Internal Fragmentation (between heap and stack)
- Hard to grow/shrink address space (once initially set)

## Segmentation Overview

__Segmentation__ is a generalized form of __base and bounds__

Each component in the address space is considered a separate segment

Each segment has its own set of base and bounds registers

### Issues/Opportunities

__Code Sharing:__ We can now share common code sections (i.e. multiple instances of programs)

__Growth Direction:__ Stack grows downward, heap grows upward; we don't want them to ever cross

### To Support

The MMU needs the following:

| Segment | Base | Size | Growth Positive | Protection |
|-|-|-|-|-|
| Code | 32 KB | 2 KB | 1 | Read-Execute | 
| Data | 34 KB | 2 KB | 1 | Read-Write |
| Heap | 36 KB | 2 KB | 1 | Read-Write |
| Stack | 28 KB | 2 KB | 0 | Read-Write |

Note: Growth direction is important to keep track of because stack
grows downward and heap grows upward

Operating system must track this info for every process and save and restore
it on each context switch

### Address Translation

Given the following 16-bit virtual address

```
         ______Offset_____
       1000 0010 0000 1010
Segment__
```

1. What is the largest possible segment size?
    - `2^14 = 16 KB`
    - Think of dorm rooms: first digit is floor, second two are room number (room 311 on floor 3)

2. What is the maximum possible offset value?
    - `2^14 - 1 = 11 1111 1111 1111 = 0x3FFF`

3. Which segment is this virtual address located in?
    - Segment `10` is __Heap__
    - Order: Code, Data, Heap, Stack

4. What is the physical address of this virtual address?
    - `Phys = Base + Offset`
    - ` = (36 KB) + (512 + 8 + 2) = 36 KB + 522 B)`

### Problems

Segmentation suffers from __external fragmentation__: the variable-sized segments lead to many
small holes

We can compact segments but that is expensive

Free-list management algorithms can only minimize fragmentation
