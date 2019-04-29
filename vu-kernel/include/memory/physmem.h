#ifndef __PHYSMEM_H
#define __PHYSMEM_H

#include "std/types.h"

#define MEM_ERR_OK 0
#define MEM_ERR_BAD_MEM 1
#define MEM_ERR_RANGE 2
#define MEM_ERR_NOMEM 3
#define MEM_ERR_DOUBLE_FREE 4

/* Ignore the first 2 MB */
#define KERN_PAGES 1024

extern void pmem_init_bitmap();
extern void *kmalloc(size_t size);

extern uint32_t kfree(uint32_t address);

extern uint32_t pmem_count_free_pages(void);
#endif
