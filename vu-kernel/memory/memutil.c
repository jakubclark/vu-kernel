/* some convenient functions - as we don't have libc, we must do
   everything ourselves */

#include "std/types.h"

uint32_t phys_mem_bytes;
uint32_t phys_num_pages;

void memcpy_a(void *dest, void *src, int size) {
  asm volatile("rep movsb"
               : "=c"((int){0})
               : "D"(dest), "S"(src), "c"(size)
               : "flags", "memory");
}

void memset_a(void *start, uint32_t val, uint32_t len) {
  asm volatile("rep stosb"
               : "=c"((int){0})
               : "D"(start), "a"(val), "c"(len)
               : "flags", "memory");
}

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
