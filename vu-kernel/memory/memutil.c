/* some convenient functions - as we don't have libc, we must do
   everything ourselves */

#include "types.h"

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
