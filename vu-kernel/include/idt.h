#include "types.h"

#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

struct idt_entry {
  unsigned short int offset_lowerbits;
  unsigned short int selector;
  unsigned char zero;
  unsigned char type_attr;
  unsigned short int offset_higherbits;
};

struct idt_ptr {
	unsigned short limit;
	unsigned long base;
} __attribute__((packed));

/* Defined in boot.S */
extern void keyboard_handler(void);
/* Defined in boot.S */
extern void load_idt(struct idt_ptr *idt_ptr);
