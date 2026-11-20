# Page Table Counting

```
Given a machine with a
X-bit virtual address
Y-bit physical address (Note, since we are talking about virtual memory,
        this number will never matter)
Z Byte page size
W Byte PTE/PDE size
```

1. How many bytes can we address?
    - Need to address every virtual address
    - `2^X Bytes`

2. How many pages do we need?
    - How many addressable bytes fit into how many pages?
    - `(2^X Bytes) / Z Bytes/page`

3. How many bits in VPN?
    - Need to address each page
    - `log_2( #2 ) = ___ bits`

4. How many bits in offset?
    - The leftover bits in the virtual address
    - `X - #3 = ___ bits`

5. How much memory does the page table require?
    - Number of pages * PTE size
    - `#2 * W = ___ Bytes`

6. How many pages does the page table require?
    - Gotten from the number of page table bytes
    - Page table bytes / page size
    - `#5 / Z = ___ pages / page table`

7. How many PTEs per page?
    - Page size / PTE size
    - `Z / W = ___ PTE / page`

8. How many PDEs per page directory?
    - `# PDEs = # page table pages` 
    - `#6 = ___ PDE / page directory`

9. How many bits in page directory index?
    - Need to reference every PDE
    - `log_2 ( #8 ) = ___ bits`

10. How many bits in page table index?
    - Recall VPN consists of PDE identifier and PTE identifier
    - Get VPN from `#3` and PDE identifier from `#9`
    - `#3 - #9 = ___ bits`

11. How much memory does the page directory require?
    - Number of PDEs * PDE size
    - `#8 * W = ___ Bytes / page directory`

12. How many pages does the page directory require?
    - Total page directory memory / Page size
    - `#11 / Z = ___ pages`
