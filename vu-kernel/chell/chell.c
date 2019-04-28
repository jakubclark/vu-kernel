#include "chell/commands/memory_cmd.h"
#include "chell/commands/multiboot_cmd.h"
#include "io/keyboard.h"
#include "io/scrn.h"
#include "memory/memutil.h"
#include "std/colors.h"
#include "std/string.h"
#include "std/types.h"

uint8_t IN_SHELL = 0;

extern uint8_t keyboard_char;
extern uint32_t min_x;

uint8_t chell_logo[162] = {
    ' ',  '_',  '_',  '_',  '_',  '_', ' ',  ' ',  '_', ' ',  ' ',  ' ', ' ',
    ' ',  ' ',  ' ',  ' ',  ' ',  ' ', ' ',  ' ',  '_', ' ',  ' ',  '_', ' ',
    '\n', '/',  ' ',  ' ',  '_',  '_', ' ',  '\\', '|', ' ',  '|',  ' ', ' ',
    ' ',  ' ',  ' ',  ' ',  ' ',  ' ', ' ',  ' ',  '|', ' ',  '|',  '|', ' ',
    '|',  '\n', '|',  ' ',  '/',  ' ', ' ',  '\\', '/', '|',  ' ',  '|', '_',
    '_',  ' ',  ' ',  ' ',  ' ',  '_', '_',  '_',  ' ', '|',  ' ',  '|', '|',
    ' ',  '|',  '\n', '|',  ' ',  '|', ' ',  ' ',  ' ', ' ',  '|',  ' ', '\'',
    '_',  ' ',  '\\', ' ',  ' ',  '/', ' ',  '_',  ' ', '\\', '|',  ' ', '|',
    '|',  ' ',  '|',  '\n', '|',  ' ', '\\', '_',  '_', '/',  '\\', '|', ' ',
    '|',  ' ',  '|',  ' ',  '|',  '|', ' ',  ' ',  '_', '_',  '/',  '|', ' ',
    '|',  '|',  ' ',  '|',  '\n', ' ', '\\', '_',  '_', '_',  '_',  '/', '|',
    '_',  '|',  ' ',  '|',  '_',  '|', ' ',  '\\', '_', '_',  '_',  '|', '|',
    '_',  '|',  '|',  '_',  '|',  '\n'};

/* Prints "user@vu" */
void print_prompt() {
  puts_col((uint8_t *)"user", LIGHTBLUE, DEFAULTBACKGROUND);
  puts_col((uint8_t *)"@", GREEN, DEFAULTBACKGROUND);
  puts_col((uint8_t *)"vu> ", RED, DEFAULTBACKGROUND);
}

/* Prints the Chell logo */
void print_logo() { puts_col(chell_logo, GREEN, DEFAULTBACKGROUND); }

/* Prints a welcome screen, to chell! */
void print_welcome() {
  print_logo();
  puts_col((uint8_t *)"Welcome to Chell, a barebones shell!\n",
           DEFAULTFOREGROUND, DEFAULTBACKGROUND);
  ;
  puts_col((uint8_t *)"help", BROWN, DEFAULTBACKGROUND);
  ;
  puts_col((uint8_t *)": list available commands\n", DEFAULTFOREGROUND,
           DEFAULTBACKGROUND);
  ;
  print_prompt();
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

      if (strcmp(cmd_buffer, (uint8_t *)"logo") == 0) {
        print_logo();
        goto cmd_end;
      }

      if (strcmp(cmd_buffer, (uint8_t *)"mem") == 0) {
        print_avail();
        goto cmd_end;
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
