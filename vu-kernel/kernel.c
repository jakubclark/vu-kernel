#define VIDEO_START 0xb8000
#define VGA_LIGHT_GRAY 7

#include "scrn.h"
#include "types.h"
#include "colors.h"
#include "init.h"

void Kernel_Main(void) {
  init();
  puts((uint8_t *)"Hello world!\nThis is the second line!\tTab\n");
  puts((uint8_t *) "Very Long Chain of Chars\rCarriage return\n");

  putsCol((uint8_t *) "Black on blue\n", BLACK, BLUE);
  putsCol((uint8_t *) "Green on cyan\n", GREEN, CYAN);
  putsCol((uint8_t *) "Red on magenta\n", RED, MAGENTA);
  putsCol((uint8_t *) "Brown on light gray\n", BROWN, LIGHTGRAY);
  putsCol((uint8_t *) "Dark gray on light blue\n", DARKGRAY, LIGHTBLUE);
  putsCol((uint8_t *) "Light green on ligt cyan\n", LIGHTGREEN, LIGHTCYAN);
  putsCol((uint8_t *) "Light red on pink\n", LIGHTRED, PINK);
  putsCol((uint8_t *) "Yellow on white\n", YELLOW, WHITE);

  printf("Formatted string: str_arg: `%s`, int_arg: `%d`, int_arg: `%d`", "ArgumentA", 59, -12);

  while (1);
}
