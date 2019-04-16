#define VIDEO_START 0xb8000
#define VGA_LIGHT_GRAY 7

#include "scrn.h"
#include "types.h"
#include "colors.h"

void Kernel_Main(void) {
  vga_init();
  puts((uint8_t *)"Hello world!\nThis is the second line!\tTab\rCarriage Return");
  putsCol((uint8_t *) "Hello!", WHITE, BLACK);
  while (1);
}
