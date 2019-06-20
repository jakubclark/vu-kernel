#ifndef __MEMTEST_CMD_H
#define __MEMTEST_CMD_H

#include "std/types.h"

extern void init_mem(uint32_t sz);
extern void mem_cmd_main();
extern void test_alloc_easy(uint32_t verbose);
extern void test_alloc_advanced(uint32_t verbose);
extern void test_alloc_oom(uint32_t verbose);

#endif
