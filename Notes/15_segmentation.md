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

