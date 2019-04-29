#ifndef __ROUTINES_H
#define __ROUTINES_H

#include "std/types.h"

/* Enable interrupts */
  void enable() { __asm__("sti" : : : "memory"); }

/* Disable interrupts */
  void disable() { __asm__ __volatile__("cli" : : : "memory"); }

/* Get the interrupt flag value from the EFLAGS register */
  void SET_IF(uint32_t IF) {
  __asm__ __volatile__(
      "pushfl                 \n"
      "popl   %%eax           \n"
      "orl    $(1 << 9), %%eax\n"
      "andl   %0, %%eax       \n"
      "pushl  %%eax           \n"
      "popfl                  \n"
      :
      : "d"((IF & 0x01) << 9));
  __asm__ __volatile__("" : : : "eax", "edx");
}

/* Get the interrupt flag value from the EFLAGS register */
  uint32_t GET_IF() {
  register uint32_t IF;
  __asm__ __volatile__(
      "pushfl                 \n"
      "popl   %%eax           \n"
      "shrl   $9, %%eax       \n"
      "andl   $1, %%eax       \n"
      : "=a"(IF)
      :);
  __asm__ __volatile__("" : : : "eax");
  return (IF);
}

/* Halt the whole system */
  void halt() { __asm__ __volatile__("cli\n hlt"); }
#endif
