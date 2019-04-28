#ifndef __MEM_H
#define __MEM_H

#include "std/types.h"

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
