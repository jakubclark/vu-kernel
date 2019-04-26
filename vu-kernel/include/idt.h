#ifndef _IDT_H
#define _IDT_H

#include "types.h"

#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

struct idt_entry {
  uint16_t offset_lowerbits;
  uint16_t selector;
  uint8_t zero;
  uint8_t type_attr;
  uint16_t offset_higherbits;
};

struct idt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

/* Initialize the Interrupt Descriptor Table */
extern void idt_init();

/* Defined in boot.asm */
extern void load_idt();

#endif
