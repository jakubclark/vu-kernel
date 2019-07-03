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

int8_t strncmp(char *str1, char *str2, uint32_t len) {
    while(len--) {
        if(*str1++ != *str2++) {
            return *(uint8_t *) (str1 - 1) - *(uint8_t *) (str2 - 1);
        }
    }
    return 0;
}

char *strchr(char *str, char c) {
    while(*str) {
        if(*str == c)
            return str;
        str++;
    }
    return NULL;
}

int32_t strlen(char *str) {
    int32_t i = 0;
    while(str[i] != '\0')
        i++;
    return i;
}

char to_upper(char c) {
    return c - 32;
}

char to_lower(char c){
  return c + 32;
}

void strcpy(char *str, char *format) {
    int i;
    for(i = 0; i < strlen(format); i++) {
        str[i] = format[i];
    }
    str[i] = '\0';
}
