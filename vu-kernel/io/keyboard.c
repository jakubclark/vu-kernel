#include "keyboard.h"
#include "basicio.h"
#include "chell.h"
#include "colors.h"
#include "scrn.h"
#include "types.h"

/* Holds the most recently pressed key */
uint8_t keyboard_char = NULL;

void kb_init(void) {
  /* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
  outbyte(0x21, 0xFD);
}

void keyboard_handler_main(void) {
  uint8_t status;
  uint8_t keycode;

  /* write EOI */
  outbyte(0x20, 0x20);

  status = inbyte(KEYBOARD_STATUS_PORT);
  /* Lowest bit of status will be set if buffer is not empty */
  if (status & 0x01) {
    keycode = inbyte(KEYBOARD_DATA_PORT);

    if (keycode == ENTER_KEY_CODE) {
      keyboard_char = '\n';
      println("");
      return;
    }

    if (keycode == BACKSPACE_KEY_CODE) {
      backspace();
      keyboard_char = '\b';
      return;
    }

    uint8_t pressed;
    /* Check if the key was released */
    if (keycode & 0x80) {
      pressed = 0;
    } else {
      pressed = 1;
    }

    if (pressed == 1) {
      keyboard_char = keyboard_map[(unsigned char)keycode];
      putchar(keyboard_char);
    }
  }
}
