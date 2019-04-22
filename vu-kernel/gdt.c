#include "gdt.h"
#include "types.h"

struct seg_descriptor gdt[3];

struct gdt_ptr gdtp;

/* gdt_reload is defined in boot.S */
extern void _gdt_reload();

void gdt_set_entry(int16_t num, uint32_t base, uint32_t limit, uint8_t segtype,
                   uint8_t descrtype, uint8_t dpl, uint8_t present,
                   uint8_t opsize, uint8_t gran) {
  gdt[num].base_low16 = (base & 0xFFFF);
  gdt[num].base_mid8 = (base >> 16) & 0xFF;
  gdt[num].base_high8 = (base >> 24) & 0xFF;
  gdt[num].limit_low16 = (limit & 0xFFFF);
  gdt[num].limit_high4 = ((limit >> 16) & 0x0F);
  gdt[num].seg_type = segtype;
  gdt[num].descr_type = descrtype;
  gdt[num].dpl = dpl;
  gdt[num].present = present;
  gdt[num].a_reserved = 0x0;
  gdt[num].zero_reserved = 0x0;
  gdt[num].operand_size = opsize;
  gdt[num].granularity = gran;
}

void gdt_install() {
  gdtp.limit = (sizeof(struct seg_descriptor) * 3) - 1;
//   gdtp.base = (uint32_t) &gdt;
  gdt_set_entry(0, 0, 0, 0, 0, 0, 0, 0, 0);
  gdt_set_entry(1, 0, 0xFFFFFFFF, 0xA, 0x1, 0x0, 0x1, 0x1, 0x1);
  gdt_set_entry(2, 0, 0xFFFFFFFF, 0x2, 0x1, 0x0, 0x1, 0x1, 0x1);
  _gdt_reload();
}
