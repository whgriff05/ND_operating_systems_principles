# TLBs, Multi-Level Paging

## Translation-Lookaside Buffer

Recall the problem of two fetches from memory
1. Get the *PTE* from physical memory
2. Get the actual data

Solution: add a __cache__ (__translation-lookaside buffer__)
 to the MMU and use it to store some address translations

The MMU uses a fully associative cache (entries can go anywhere, but the
 whole cache must be searched) and usually stores entries in the following format

```
VPN -> PFN | Other Bits
```

where other bits could be __valid bits__, __protection bits__, __dirty bits__, __address
space identifier__, etc.

### Aside on Locality

__Spatial Locality:__ Accessing nearby elements already fetched by a cache
- Ex: accessing elements in array sequentially

__Temporal Locality:__ Accessing the same element over and over already fetched by a cache
- Ex: Accessing the same variable in a loop

### TLBs and Context Switch

On each context switch, the OS must either

1. Flush the TLB, marking all entries as invalid (avoid using wrong translations)
2. Set the ASID (address space identifier) register to the current PID so the 
MMU knows which entries are valid

### Handling Misses

Hardware Side:
- MMU walks page table, finds PTE, and updates TLB itself with no software interaction
- happens on x86 and ARM

Software Side:
- MMU raises a fault, triggers a trap handler, and OS looks up PTE and updates TLB
- happens on SPARC and MIPS

## Multi-Level Page Tables

Problem: array-based (linear) page tables are __too big__ and take up too much memory

What if we divide our page table into pages?

For example, what if we combined every 4 PTEs into its own page \
Then, we could create a page directory to keep track of page table's own pages 
and any invalid pages of the page table (completely invalid PTE blocks) remain unallocated

## Practice Problem
```
14-bit virtual address
16-bit physical address --> since we are talking about VIRTUAL memory, this will never matter
64 B page size
4 B PTE/PDE
```

1. How many bytes can we address
    - `2^14 B = 16 KB`
2. How many pages do we need
    - `2^14 B / 64B/page (2^6) = 2^8 pages`
3. How many bits in VPN
    - `log_2 (2^8) = 8 bits`
4. How many bits in offset
    - `log_2 (2^6) = 6 bits`
    - `14 bits - 8 bits = 6 bits`
5. How much memory does page table require
    - Number of pages * PTE size
    - `2^8 pages * 2^2 B/PTE = 2^10 B = 1 KB` 
6. How many pages does page table require
    - Page Table Bytes / Page Size
    - `2^10 B / 2^6 B/page = 2^4 pages`
7. How many PTEs per page
    - Page Size / Page Table Bytes
    - `2^6 B/page / 2^2 B/PTE = 2^4 PTE / page`
8. How many PDEs in page directory
    - need a PDE for each page table page (see question 6)
    - `16 PDEs`
9. How many bits in page directory index
    - `log_2 (2^4) = 4 bits`
    - Page Directory Index (PDI)
10. How many bits in page table index
    - VPN consists of PDE identifier and PTE identifier (see question 3)
    - `8 bits - 4 bits = 4 bits`
    - Page Table Index (PTI)
11. How much memory does page directory require
    - Number of PDEs * PDE Size
    - `2^4 PDE * 4 B/PDE = 2^6 B = 64 B`
12. How many pages does page directory require
    - Page Directory Size / Page Size
    - `2^6 B / 2^6 B/page = 1 page`
