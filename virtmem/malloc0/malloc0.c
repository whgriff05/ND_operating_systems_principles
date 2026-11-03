/* malloc.c: simple memory allocator */

#include <assert.h>
#include <unistd.h>

/*
 *
 * Pros:
 * - "works", simple
 *
 * Cons:
 * - Never release any memory
 * - Never reuse any memory
 * - No alignment
 *
 */

/* Allocate space */

void *malloc(size_t size) {
    void* block = sbrk(size);

    if (block == (void*)-1) {
        return NULL;
    }

    return block;
}

/* Reclaim space */

void free(void *ptr) {
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
