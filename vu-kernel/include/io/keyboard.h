#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "std/types.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define ENTER_KEY_CODE 0x1C
#define BACKSPACE_KEY_CODE 0xE

/* The keyboard map is from:
 * http://www.osdever.net/bkerndev/Docs/keyboard.htm
 */
extern uint8_t keyboard_map[128];

/* Initialize the keyboard driver */
extern void kb_init();

/* Defined in boot.asm
 * Reads the pressed/released button.
 * If a button was pressed, its ascii
 * value is stored in `keyboard_char`.
 * Releases are ignored.
 * Valid characters are defined in `keyboard_map` */
extern void keyboard_handler(void);

/* Block until a key is pressed, and return that char */
extern uint8_t get_char(void);

#endif
