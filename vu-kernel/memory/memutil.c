/* some convenient functions - as we don't have libc, we must do
   everything ourselves */

#include "std/types.h"

uint32_t phys_mem_bytes;
uint32_t phys_num_pages;

uint8_t *memcpy(unsigned char *dest, const unsigned char *src, int count) {
  int i;
  for (i = 0; i < count; i++) dest[i] = src[i];
  return dest;
}

uint8_t *memset(void *dst, unsigned char val, int count) {
  if (count) {
    char *d = dst;
    do {
      *d++ = val;
    } while (--count);
  }
  return dst;
}

void set_bit(uint32_t *dest, uint16_t bit) {
  uint32_t res = *dest;
  res |= (1 << bit);
  *dest = res;
}

void clear_bit(uint32_t *dest, uint16_t bit) {
  uint32_t res = *dest;
  res &= ~(1 << bit);
  *dest = res;
}

uint16_t get_bit(uint32_t src, uint16_t bit) { return (src >> bit) & 1; }
