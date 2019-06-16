#ifndef __STRING_H
#define __STRING_H
#include "std/types.h"

/* Compares `s1` and `s2`. Returns 0 if equivalent, -1 otherwise.
 * BOTH STRINGS MUST END WITH '\0'! */
extern int8_t strcmp(const uint8_t *s1, const uint8_t *s2);

extern int8_t strncmp(char *str1, char *str2, uint32_t len);

/* Attempt to find `c` in the string `str` */
extern char *strchr(char *str, int c);

extern int32_t strlen(char *str);

extern char to_upper(char c);

extern char to_lower(char c);

extern void strcpy(char *str, char *format);

#endif
