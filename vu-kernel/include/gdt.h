#ifndef __GDT_H
#define __GDT_H

#include "types.h"
#define GDT_SIZE 3
#define GDT_MEM_LOW 0
#define GDT_MEM_LEN 0xFFFFFFFF

#define GDT_EXE 0x8
#define GDT_READ 0x2
#define GDT_WRITE 0x2

/* Kernel code always runs in ring 0 */
#define DPL_KERNEL 0

/* GDT entry numbers */
enum { _GDT_NULL, _KERNEL_CS, _KERNEL_DS };

/* GDT entry offsets */
#define GDT_NULL (_GDT_NULL << 3)
#define KERNEL_CS (_KERNEL_CS << 3)
#define KERNEL_DS (_KERNEL_DS << 3)

struct gdt_entry {
  /* Low 8 bits of the "limit", or length of memory this descriptor refers to.
   */
  uint16_t limit_low;
  uint16_t base_low;   /* 'Low' 16-bits of the base */
  uint8_t base_middle; /* 'middle' 8 bits of the base */

  uint8_t
      type : 4; /* Flags for type of memory this descriptor describes */
  uint8_t one : 1;
  uint8_t dpl : 2;     /* Descriptor privilege level - Ring level */
  uint8_t present : 1; /* 1 for any valid GDT entry */

  uint8_t limit : 4; /* Top 4 bytes of 'limit' */
  uint8_t avilable : 1;
  uint8_t zero : 1;
  uint8_t op_size : 1; /* Selects between 16-bit and 32-bit */
  uint8_t gran : 1; /* If this bit is set, then 'limit' is a count of 4K
                             blocks, not bytes */

  uint8_t base_high; /* High 8 bits of the base */
} __attribute__((packed));

struct gdt_ptr {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

#define GDT_ENTRY(gdt_type, gdt_base, gdt_limit, gdt_dpl)                      \
  {                                                                            \
    .limit_low = (((gdt_limit) >> 12) & 0xFFFF),                               \
    .base_low = ((gdt_base)&0xFFFF),                                           \
    .base_middle = (((gdt_base) >> 16) & 0xFF), .type = gdt_type, .one = 1,    \
    .dpl = gdt_dpl, .present = 1, .limit = ((gdt_limit) >> 28), .avilable = 0, \
    .zero = 0, .op_size = 1, .gran = 1,                                        \
    .base_high = (((gdt_base) >> 24) & 0xFF),                                  \
  }

/* defined in boot.asm */
extern void load_gdt(struct gdt_ptr *gdt_ptr);

/* defined in gdt.c */
extern void gdt_init(void);

#endif
