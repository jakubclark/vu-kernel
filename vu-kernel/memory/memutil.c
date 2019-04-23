/* some convenient functions - as we don't have libc, we must do
   everything ourselves */

#include "types.h"

uint32_t mem_start_address = 0x100000;
uint32_t mem_end_address;
uint32_t mem_use_address = 0x800000;

uint8_t *memcpy(unsigned char *dest, const unsigned char *src, int count) {
  int i;
  for (i = 0; i < count; i++) dest[i] = src[i];
  return dest;
}

uint8_t *memset(unsigned char *dest, unsigned char val, int count) {
  int i;
  for (i = 0; i < count; i++) dest[i] = val;
  return dest;
}
