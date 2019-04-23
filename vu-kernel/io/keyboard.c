#include "keyboard.h"
#include "basicio.h"
#include "chell.h"
#include "colors.h"
#include "scrn.h"
#include "types.h"

/* Holds the most recently pressed key */
uint8_t keyboard_char = NULL;

/* Whether we are in the shell */
extern uint8_t IN_SHELL;

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
      println("");
      keyboard_char = '\r';
      if (IN_SHELL == 1) print_prompt();
      return;
    }

    if (keycode == BACKSPACE_KEY_CODE) {
      backspace();
      keyboard_char = '\b';
      return;
    }

    /* Check if the key was released */
    if (keycode & 0x80) {
      keyboard_char = NULL;
      return;
    }

    keyboard_char = keyboard_map[(unsigned char)keycode];
    putchar(keyboard_char);
  }
}

uint8_t get_char(void) {
  int i;
  while (keyboard_char == NULL) {
    for (i = 0; i < 100; i++) {
      continue;
    }
  }
  return keyboard_char;
}
