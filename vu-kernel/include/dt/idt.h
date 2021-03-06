#ifndef _IDT_H
#define _IDT_H

#include "std/types.h"

#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

typedef struct idt_entry {
  uint16_t offset_lowerbits;
  uint16_t selector;
  uint8_t zero;
  uint8_t type_attr;
  uint16_t offset_higherbits;
} idt_entry_t;

typedef struct idt_ptr {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) idt_ptr_t;

/* Initialize the Interrupt Descriptor Table */
extern void idt_init();

/* Defined in boot.asm */
extern void load_idt();

/* Install an Interrupt Request Handler */
extern void install_ir(uint32_t i, uint16_t type_attr, uint16_t selector,
                       void *irq_func);

#endif
