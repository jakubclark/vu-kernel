#include "std/types.h"

/* We use inbyte() to read from the I/O ports (e.g., to get data from
 * the keyboard). */
uint8_t inbyte(uint16_t _port) {
  unsigned char rv;
  __asm__ __volatile__("inb %1, %0" : "=a"(rv) : "dN"(_port));
  return rv;
}

/* We use outbyte() to write to the I/O ports (e.g., the screen). */
void outbyte(uint16_t _port, uint16_t _data) {
  __asm__ __volatile__("outb %1, %0" : : "dN"(_port), "a"(_data));
}
