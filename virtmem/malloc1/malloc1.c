/* malloc.c: POSIX API Implementation */

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Block Constants */

#define ALIGNMENT       (sizeof(double))
#define ALIGN(size)     (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define SBRK_FAILURE    ((void *)(-1))

/* Block Macros */

#define BLOCK_FROM_POINTER(ptr) \
    (Block *)((intptr_t)(ptr) - sizeof(Block))

/* Block Structure */

typedef struct block Block;
struct block {
    size_t   capacity;      /* Number of bytes allocated */
    size_t   size;          /* Number of bytes used */
    Block *  prev;          /* Pointer to previous block */
    Block *  next;          /* Pointer to next block */
    char     data[];        /* "Label" to the block contents - 0 bytes so that the block data can be accessed as an array*/
};                          /* sizeof(Block) = 32  (good for alignment) */

Block * block_allocate(size_t size) {
    intptr_t allocated = sizeof(Block) + ALIGN(size);
    Block* block = sbrk(allocated);

    if (block == SBRK_FAILURE) {
        return NULL;
    }

    block->capacity = ALIGN(size);
    block->size = size;
    block->next = block; 
    block->prev = block; 

    return block;
}

Block * block_detach(Block *block) {
    /* Update prev and next blocks to reference each other */
    Block* prev = block->prev;
    Block* next = block->next;

    prev->next = next;
    next->prev = prev;

    block->prev = NULL;
    block->next = NULL;

    return block;
}

/* Free List Global Variable */

Block FreeList = {0, 0, &FreeList, &FreeList};

/* Free List Functions */

Block * free_list_search(size_t size) {
    /* Perform first fit algorithm */
    for (Block* curr = FreeList.next; curr != &FreeList; curr = curr->next) {
        if (curr->capacity >= size) {
            return curr;
        }
    }

    return NULL;
}

void    free_list_insert(Block *block) {
    /* Append to tail */

    // Insert in sorted order to perform coalescence

    Block* tail = FreeList.prev;

    tail->next = block;
    FreeList.prev = block;

    block->prev = tail;
    block->next = &FreeList;

    // To combat external fragmentation: block_merge()
    // 2 merges: merge with block after, and merge with block before
}

/* POSIX API Functions */

/**
 * Allocate specified amount memory.
 * @param   size    Amount of bytes to allocate.
 * @return  Pointer to the requested amount of memory.
 **/
void *malloc(size_t size) {
    /* Handle empty size */
    if (size == 0) return NULL;

    /* Search free list for any available block with matching size */
    Block* block = free_list_search(size);

    /* Could not find free block or allocate a block, so just return NULL */
    if (!block) {
        block = block_allocate(size); // Calls sbrk, grows heap
    } else {
        // To combat internal fragmentation: block_split()
        block = block_detach(block);  // Remove from free list since in use
    }

    if (block == NULL) return NULL;

    /* Return data address associated with block */
    return block->data;
}

/**
 * Release previously allocated memory.
 * @param   ptr     Pointer to previously allocated memory.
 **/
void free(void *ptr) {
    if (!ptr) return;

    Block* block = BLOCK_FROM_POINTER(ptr);

    free_list_insert(block);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
