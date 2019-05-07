#ifndef __PAGING_H
#define __PAGING_H

#include "std/types.h"

/* The page is present in memory */
#define PRESENT 0x1
/* Page is read and write */
#define READWRITE 0x2
/* Anyone can access */
#define USERMODE 0x4
/* Write-through cachine is enabled */
#define WRITETHROUGH 0X8
/* Page is not cached */
#define CACHEDISABLE 0x10
/* Page has been accessed */
#define ACCESSED 0x20

extern void initialize_paging(uint32_t phys_num_pages);

extern void page_fault(registers_t regs);

extern void page_fault_main(void);

#endif
