#include "colors.h"
#include "keyboard.h"
#include "scrn.h"
#include "types.h"

uint8_t IN_SHELL = 0;

void print_prompt() {
  puts_col((uint8_t *)"user", LIGHTBLUE, DEFAULTBACKGROUND);
  puts_col((uint8_t *)"@", GREEN, DEFAULTBACKGROUND);
  puts_col((uint8_t *)"vu> ", RED, DEFAULTBACKGROUND);
}

void print_welcome() {
  println("help: list available commands");
  print_prompt();
}

void chell_main() {
  uint8_t char_;
  IN_SHELL = 1;
  println("Welcome to Chell, a barebones shell!");
  while (1) {
    print_welcome();
    printf("Awaiting for a keypress");
    char_ = get_char();
    printf("Received a keypress");
    putchar(char_);
  }
  IN_SHELL = 0;
}
