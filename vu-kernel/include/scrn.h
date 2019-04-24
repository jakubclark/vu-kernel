#ifndef __SCRN_H
#define __SCRN_H

#include "types.h"

/* Print `c` to the screen */
extern void putchar(uint8_t c);

/* Print `c` to the screen with `color` as the background/foreground */
extern void putcharCol(uint8_t c, uint16_t color);

/* Print `str` to the screen */
extern void puts(uint8_t *str);

/* Print `str` with `foreground` and `background` colors*/
extern void puts_col(uint8_t *str, uint8_t foreground, uint8_t background);

/* Backspace */
extern void backspace();

/* Initialize the screen*/
extern void vga_init();

/* Clear the screen */
extern void clear();

/* print using `fmt` */
extern void printf(const char *format, ...);

/* Print `str` and append `\n` */
extern void println(const char *str);

/* convert an int `d` in base `base` to a string stored in `buf` */
extern void itoa(char *buf, int base, int d);

/* Set the default color of the terminal */
extern void set_default_color(uint8_t fg, uint8_t bg);

#endif
