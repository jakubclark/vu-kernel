#include "scrn.h"
#include "basicio.h"
#include "chell.h"
#include "keyboard.h"
#include "memutil.h"
#include "types.h"

#define COLS 80
#define ROWS 25
#define VGA_START 0xB8000
#define PRINTABLE(c) (c >= ' ')

uint16_t *Scrn;                         // screen area
int Curx, Cury = 0;                     // current cursor coordinates
uint16_t EmptySpace = 0x0F << 8 | 0x20; /* 0x20 is ascii value of space */
uint16_t DefaultColor = 0x0F;

void scroll(void) {
  int dist = Cury - ROWS + 1;

  if (dist > 0) {
    uint8_t *newstart = ((uint8_t *)Scrn) + dist * COLS * 2;
    int bytesToCopy = (ROWS - dist) * COLS * 2;
    uint16_t *newblankstart = Scrn + (ROWS - dist) * COLS;
    int bytesToBlank = dist * COLS * 2;
    memcpy((uint8_t *)Scrn, newstart, bytesToCopy);
    memset((uint8_t *)newblankstart, EmptySpace, bytesToBlank);
  }
}

void putchar(uint8_t c) { putcharCol(c, DefaultColor); }

void putcharCol(uint8_t c, uint16_t color) {
  switch (c) {
    case '\t':
      Curx = ((Curx + 4) / 4) * 4;
      break;
    case '\r':
      Curx = 0;
      break;
    case '\n':
      Curx = 0;
      Cury++;
      break;
    case 0x08:
      if (Curx > 0) Curx--;
      break;
    default:
      if (PRINTABLE(c)) {
        Scrn[Cury * COLS + Curx] = (color << 8) | c;
        Curx++;
      }
  }

  // if we have reached the end of the line, move to the next
  if (Curx >= COLS) {
    Curx = 0;
    Cury++;
  }

  scroll();
}

void puts(unsigned char *str) {
  while (*str) {
    putchar(*str);
    str++;
  }
}

void putsCol(unsigned char *str, uint8_t foreground, uint8_t background) {
  uint16_t color = background << 4 | foreground;
  while (*str) {
    putcharCol(*str, color);
    str++;
  }
}

void backspace() {
  if (Curx > PROMPT_SIZE) Curx--;
  putchar(EmptySpace);
  if (Curx > PROMPT_SIZE) Curx--;
}

void itoa(char *buf, int base, int d) {
  char *p = buf;
  char *p1, *p2;
  unsigned long ud = d;
  int divisor = 10;

  /* If %d is specified and D is minus, put `-' in the head.  */
  if (base == 'd' && d < 0) {
    *p++ = '-';
    buf++;
    ud = -d;
  } else if (base == 'x')
    divisor = 16;

  /* Divide UD by DIVISOR until UD == 0.  */
  do {
    int remainder = ud % divisor;

    *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
  } while (ud /= divisor);

  /* Terminate BUF.  */
  *p = 0;

  /* Reverse BUF.  */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2) {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }
}

void printf(const char *format, ...) {
  char **arg = (char **)&format;
  int c;
  char buf[20];

  arg++;

  while ((c = *format++) != 0) {
    if (c != '%')
      putcharCol(c, DefaultColor);
    else {
      char *p;

      c = *format++;
      switch (c) {
        case 'd':
        case 'u':
        case 'x':
          itoa(buf, c, *((int *)arg++));
          p = buf;
          goto string;
          break;

        case 's':
          p = *arg++;
          if (p == NULL) p = "(null)";

        string:
          while (*p) putcharCol(*p++, DefaultColor);
          break;

        default:
          putcharCol(*((int *)arg++), DefaultColor);
          break;
      }
    }
  }
}

void println(const char *str) {
  while (*str) {
    putchar(*str);
    str++;
  }
  putchar('\n');
}

void clear() {
  int i;
  for (i = 0; i < ROWS * COLS; i++) putcharCol(' ', DefaultColor);
  Curx = Cury = 0;
  Scrn[i] = EmptySpace;
}

void vga_init(void) {
  Scrn = (unsigned short *)VGA_START;
  clear();
}

void set_default_color(uint8_t fg, uint8_t bg) { DefaultColor = bg << 4 | fg; }