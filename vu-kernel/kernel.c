#include "chell/chell.h"
#include "chell/commands/multiboot_cmd.h"
#include "dt/gdt.h"
#include "dt/idt.h"
#include "io/keyboard.h"
#include "io/scrn.h"
#include "multiboot.h"
#include "std/colors.h"
#include "std/types.h"

void print_colored_text() {
  puts((uint8_t *)"Hello world!\nThis is the second line!\tTab\n");
  puts((uint8_t *)"Very Long Chain of Chars\rCarriage return\n");

  puts_col((uint8_t *)"Black on blue\n", BLACK, BLUE);
  puts_col((uint8_t *)"Green on cyan\n", GREEN, CYAN);
  puts_col((uint8_t *)"Red on magenta\n", RED, MAGENTA);
  puts_col((uint8_t *)"Brown on light gray\n", BROWN, LIGHTGRAY);
  puts_col((uint8_t *)"Dark gray on light blue\n", DARKGRAY, LIGHTBLUE);
  puts_col((uint8_t *)"Light green on ligt cyan\n", LIGHTGREEN, LIGHTCYAN);
  puts_col((uint8_t *)"Light red on pink\n", LIGHTRED, PINK);
  puts_col((uint8_t *)"Yellow on white\n", YELLOW, WHITE);

  printf("Formatted string: str_arg: `%s`, int_arg: `%d`, int_arg: `%d`",
         "ArgumentA", 59, -12);
}

void init() {
  vga_init();
  gdt_init();
  idt_init();
  kb_init();
}

void kernel_main(uint32_t magic, multiboot_info_t *mbi) {
  init();
  set_addrs(magic, mbi);
  chell_main();

  while (1)
    ;
}
