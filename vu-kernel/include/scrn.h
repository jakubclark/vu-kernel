#ifndef __SCRN_H
#define __SCRN_H

#include "types.h"

/* Clear the screen */
extern void clear();

/* Print the `c` to the screen */
extern void putchar(uint8_t c);

/* Print the `c` to the screen with `background` and `foreground` */
extern void putcharCol(uint8_t c, uint16_t color);

/* Print the `str` to the screen */
extern void puts(uint8_t *str);

/* Print the `str` with `foreground` and `background` */
extern void putsCol(uint8_t *str, uint16_t foreground, uint16_t background);

/* Initialize the screen*/
extern void vga_init();

/* print using `fmt` */
void printf(const char *format, ...);

/* convert an int `d` in base `base` to a string stored in `buf` */
void itoa(char *buf, int base, int d);

#endif
