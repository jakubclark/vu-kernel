#ifndef __MEM_H
#define __MEM_H

#include "types.h"

extern uint8_t *memcpy(uint8_t *dest, const uint8_t *src, int count);
extern uint8_t *memset(uint8_t *dest, uint8_t val, int count);

#endif
