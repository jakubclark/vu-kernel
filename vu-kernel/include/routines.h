#ifndef __ROUTINES_H
#define __ROUTINES_H

#include "std/types.h"

/* Enable interrupts */
extern void enable();

/* Disable interrupts */
extern void disable();

/* Set the interrupt flag value from the EFLAGS register */
extern void SET_IF(uint32_t IF);

/* Get the interrupt flag value from the EFLAGS register */
extern uint32_t GET_IF();

/* Halt the whole system */
extern void halt();

/* Something went wrong, PANIC */
extern void PANIC(uint8_t *str);

#endif
