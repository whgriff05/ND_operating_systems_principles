# When are counters updated in Project 03?

## `malloc()`
- `free_list_search()` -> REUSES + 1
- if block
    - `block_split()` -> SPLITS + 1, BLOCKS + 1
- else
    - `block_allocate()` -> HEAP_SIZE + allocated, BLOCKS + 1, GROWS + 1
- -> MALLOCS + 1, REQUESTED + size

## `free()`
- -> FREES + 1
- if `block_release()`
    - -> BLOCKS - 1, SHRINKS + 1, HEAP_SIZE - allocated
- else
    - `free_list_insert()`
        - `block_merge()` -> MERGES + 1, BLOCKS - 1
        - `block_merge()` -> MERGES + 1, BLOCKS - 1


## `calloc()`
- `malloc()` -> (see above)
- -> CALLOCS + 1

## `realloc()`
- if not ptr
    - `malloc(new size)` -> (see above)
- if not size
    - `free(ptr)` -> (see above)

- else
    - `malloc(new size)` -> (see above)
    - `free(old ptr)` -> (see above)
    - -> REALLOCS + 1
