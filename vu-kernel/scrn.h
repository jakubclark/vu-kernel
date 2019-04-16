#ifndef __SCRN_H
#define __SCRN_H

#include "types.h"

/* Clear the screen */
extern void clear();
/* Print the `c` to the screen */
extern void putchar(uint8_t c);
/* Print the `str` to the screen */
extern void puts(uint8_t *str);
/* Print the `str` with `background` and `foreground` */
extern void putsCol(uint8_t *str, colorCode background, colorCode foreground);;
/* Initialize the screen*/
extern void vga_init();

void printf(const char *format, ...);
void itoa(char *buf, int base, int d);

#endif
