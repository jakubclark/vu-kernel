#ifndef __MEMTEST_CMD_H
#define __MEMTEST_CMD_H

#include "std/types.h"

extern void init_mem(uint32_t sz);

extern void test_alloc_easy();
extern void test_alloc_advanced();
extern void test_alloc_oom();
extern void test_all();

#endif
