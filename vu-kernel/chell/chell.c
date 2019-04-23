#include "colors.h"
#include "scrn.h"
#include "types.h"
#include "keyboard.h"

void print_prompt(){
    set_default_color(LIGHTBLUE, BLACK);
    printf("user");
    set_default_color(GREEN, BLACK);
    printf("@");
    set_default_color(RED, BLACK);
    printf("cs> ");
    set_default_color(WHITE, BLACK);
}

void print_welcome(){
    set_default_color(WHITE, BLACK);
    println("help: list available commands");
    print_prompt();
}

void chell_main() {
  uint8_t char_;
  println("Welcome to Chell, a barebones shell!");
  while (1) {
    print_welcome();
    char_ = get_char();
    putchar(char_);
  }
}
