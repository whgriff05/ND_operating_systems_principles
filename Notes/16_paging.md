# Paging

## Overview

Rather than chopping up memory into variable-sized pieces, we divide it
into __fixed-size chunks__
- Each process has an address space that consists of __fixed-size virtual
pages__
- Each virtual page maps to a __physical page frame__ (of the same size)
- *typical page size is 4 KB, `BUFSIZ` is 8 KB*

## Page Table

Each process has a __page table__ that stores the mapping of each virtual page 
number (VPN) to the corresponding physical page frame (PFN)

| Bit | Information | 
| - | - |
| Valid | Is the mapping valid? |
| Protection | Can we read, write, execute data in page? |
| Present | Is the page in __physical memory__? |
| Reference | Has the page been accessed (recently)? |

In addition to the mapping, each page table entry (PTE) may contain the bits
 of information above

## Address Translation
Given the following virtual address 36, answer the following questions 
(assuming page table on Reading 09):
```
       ____Offset
   00100100
VPN____
```

1. Which page is this address located on?
    - Page 2

2. Which page frame is the address located on?
    - Page Frame 5

3. What is the offset of this address?
    - `0100` -> `0x4`

4. What is the physical address?
    - Base in this case is `PFN x Page Size`
    - `0101 0100` -> `0x54`

## Memory Usage

Suppose we had a 32-bit machine with 4 KB pages

1. How many pages can a process have?
    - Size of Address Space divided by Size of Page
    - `2^32 / 2^12 = 2^20 (~1 M) pages`

2. How many bits do we need for the VPN?
    - `log_2 (2^20) = 20 bits`
    - We need 20 bits to represent 2^20 pages

3. How many bits do we need for the offset?
    - `32 - log_2 (2^20) = 32 - 20 = 12 bits`
    - `log_2 (2^12) = 12 bits`
    - Offset is just the remainder of bits OR how many bits
    to represent the page size

4. How large is the page table (assuming each PTE is 4 B)?
    - Number of Pages multiplied by Size of PTE
    - `2^20 pages * 2^2 B = 2^22 B = 4 MB`

## Problems

- __Overhead:__ Each process requires a page table, and each table can be quite large, 
leading to high memory usage
- __Indirection:__ Each memory access requires a page table entry lookup, meaning we have 2 
physical memory accesses per virtual address
    - Quasi-solution: Use a __cache__ (Translation-Lookaside Buffer: Comp Arch!)
