#include "io/keyboard.h"
#include "chell/chell.h"
#include "io/basicio.h"
#include "io/scrn.h"
#include "std/colors.h"
#include "std/types.h"

/* Holds the most recently pressed key */
uint8_t keyboard_char = NULL;

void kb_init(void) {
  /* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
  outbyte(0x21, 0xFD);
}

/* Read the keycode. If it was pressed (not released),
 * set keyboard_char to the associate character.
 * For a list of "accepted" characters, see `keyboard_map. */
void keyboard_handler_main(void) {
  uint8_t status, keycode;

  /* write EOI */
  outbyte(0x20, 0x20);

  status = inbyte(KEYBOARD_STATUS_PORT);
  /* Lowest bit of status will be set if buffer is not empty */
  if (status & 0x01) {
    keycode = inbyte(KEYBOARD_DATA_PORT);

    if (keycode == ENTER_KEY_CODE) {
      keyboard_char = '\n';
      return;
    }

    if (keycode == BACKSPACE_KEY_CODE) {
      keyboard_char = '\b';
      return;
    }

    /* Check if the key was released */
    if (keycode & 0x80) {
      keyboard_char = NULL;
    } else {
      keyboard_char = keyboard_map[(unsigned char)keycode];
    }
  }
}

uint8_t get_char() {
  while (keyboard_char == NULL) {
    printf("");
    continue;
  }
  return keyboard_char;
}
