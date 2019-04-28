#include "std/types.h"

int8_t strcmp(const uint8_t* s1, const uint8_t* s2) {
  while (*s1 == *s2) {
    if (*s1 == '\0' || *s2 == '\0') break;

    s1++;
    s2++;
  }

  if (*s1 == '\0' && *s2 == '\0') return 0;
  return -1;
}