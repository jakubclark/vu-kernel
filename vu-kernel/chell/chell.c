#include "colors.h"
#include "keyboard.h"
#include "memutil.h"
#include "multiboot_cmd.h"
#include "scrn.h"
#include "string.h"
#include "types.h"

uint8_t IN_SHELL = 0;

extern uint8_t keyboard_char;
extern uint32_t min_x;

/* Prints "user@vu" */
void print_prompt() {
  puts_col((uint8_t *)"user", LIGHTBLUE, DEFAULTBACKGROUND);
  puts_col((uint8_t *)"@", GREEN, DEFAULTBACKGROUND);
  puts_col((uint8_t *)"vu> ", RED, DEFAULTBACKGROUND);
}

/* Prints a welcome screen, to chell! */
void print_welcome() {
  println("Welcome to Chell, a barebones shell!");
  println("help: list available commands");
  print_prompt();
  set_default_color(DEFAULTFOREGROUND, DEFAULTBACKGROUND);
}

void chell_main() {
  uint8_t char_;

  uint8_t cmd_buffer[256];
  uint8_t cmd_buffer_index = 0;
  cmd_buffer[cmd_buffer_index] = '\0';

  IN_SHELL = 1;

  /* The len of "user@vu>"*/
  min_x = 8;
  print_welcome();

  while (1) {
  loop:
    printf("");
    char_ = get_char();
    putchar(char_);

    if (char_ == '\n') {
      cmd_buffer[cmd_buffer_index] = '\0';

      if (cmd_buffer_index == 0) goto prompt;

      if (strcmp(cmd_buffer, (uint8_t *)"help") == 0) {
        printf("HELP COMMAND GOES HERE");
        goto cmd_end;
      }

      if (strcmp(cmd_buffer, (uint8_t *)"index") == 0) {
        printf("INDEX: `%d`", cmd_buffer_index);
        goto cmd_end;
      }

      if (strcmp(cmd_buffer, (uint8_t *)"multiboot") == 0) {
        print_multiboot_info();
        goto cmd_end;
      }

      if (strcmp(cmd_buffer, (uint8_t *)"clear") == 0) {
        clear();
        goto prompt;
      }

      printf("UNKOWN COMMAND: `%s`", cmd_buffer);
      goto cmd_end;
    } else if (char_ == '\b') {
      cmd_buffer[cmd_buffer_index] = '\0';
      if (cmd_buffer_index != 0) {
        cmd_buffer_index--;
      }
      goto end;
    } else {
      if (cmd_buffer_index < 255) {
        cmd_buffer[cmd_buffer_index] = char_;
        cmd_buffer_index++;
      } else {
        printf("\nCommand buffer overflow, please re-enter your command");
        goto cmd_end;
      }
      goto end;
    }

  cmd_end:
    println("");
    cmd_buffer_index = 0;
    memset(cmd_buffer, 0, 256);
    goto prompt;

  prompt:
    print_prompt();
    cmd_buffer_index = 0;
    goto end;

  end:
    keyboard_char = NULL;
    goto loop;
  }
  IN_SHELL = 0;
}
