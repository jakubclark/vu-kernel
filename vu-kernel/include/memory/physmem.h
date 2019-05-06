#ifndef __PHYSMEM_H
#define __PHYSMEM_H

#include "std/types.h"

/* Convert address to page number/index */
#define ATP(addr) addr / PAGE_SIZE;

/* Isn't actually allocated */
#define MEM_ERR_BAD_MEM 1
/* Attempting to allocate a frame/page that is too high */
#define MEM_ERR_RANGE 2
/* Ran out of memory */
#define MEM_ERR_NOMEM 3
/* Attempt to free already free memory */
#define MEM_ERR_DOUBLE_FREE 4

/* enough for 4 GB */
#define MEM_BLOCKS 1048576
#define PAGES MEM_BLOCKS / PAGE_BITS

#define FREE 0
#define ALLOCATE 1
#define RESERVED 2

/* Ignore the first 2 MB */
#define KERN_PAGES 1024

/* Initialize the frames/pages bitmap */
extern void physmem_init();
/* Request a pointer to continous frames/pages of at least `size` bytes */
extern void *kmalloc(uint32_t size);
/* Free the frames/pages referenced by `address` */
extern void kfree(uint32_t address);
/* Count the number of free pages */
extern uint32_t count_free_pages(void);
#endif
