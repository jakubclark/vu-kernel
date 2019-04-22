#include "keyboard.h"
#include "basicio.h"
#include "scrn.h"
#include "types.h"

void kb_init(void) {
  /* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
  outbyte(0x21, 0xFD);
}

void keyboard_handler_main(void) {
  unsigned char status;
  char keycode;

  /* write EOI */
  outbyte(0x20, 0x20);

  status = inbyte(KEYBOARD_STATUS_PORT);
  /* Lowest bit of status will be set if buffer is not empty */
  if (status & 0x01) {
    keycode = inbyte(KEYBOARD_DATA_PORT);
    if (keycode < 0)
      return;

    if (keycode == ENTER_KEY_CODE) {
      puts((uint8_t *)"\n");
    } else if (keycode == BACKSPACE_KEY_CODE) {
      backspace();
    } else {
      putchar(keyboard_map[(unsigned char)keycode]);
    }
  }
}
