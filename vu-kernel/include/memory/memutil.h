#ifndef __MEMUTIL_H
#define __MEMUTIL_H

#include "std/types.h"

/* The total number of bytes in RAM */
extern uint32_t phys_mem_bytes;
/* The total number of physical pages
 * Ideally, this is `phys_mem_bytes` / `PAGE_SIZE` */
extern uint32_t phys_num_pages;

/* copy `count` bytes to `dest` from `src` */
extern uint8_t *memcpy(uint8_t *dest, const uint8_t *src, int count);
/* set `count` bytes to `dest` with a value of `val` */
extern uint8_t *memset(uint8_t *dest, uint8_t val, int count);
/* set bit `bit` at `dest` */
extern void set_bit(uint32_t *dest, uint16_t bit);
/* clear `bit` from `dest` */
extern void clear_bit(uint32_t *dest, uint16_t bit);
/* get bit `bit` from `src` */
extern uint16_t get_bit(uint32_t src, uint16_t bit);

#endif
