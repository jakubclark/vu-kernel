#include "gdt.h"
#include "types.h"

struct gdt_entry gdt[3];

struct gdt_entry GDT[GDT_SIZE] = {
    [_GDT_NULL] = {0 /* NULL GDT entry - Required */},
    [_KERNEL_CS] = GDT_ENTRY(GDT_EXE | GDT_READ, 0, 0xFFFFFFFF, DPL_KERNEL),
    [_KERNEL_DS] = GDT_ENTRY(GDT_WRITE, 0, 0xFFFFFFFF, DPL_KERNEL)};

void gdt_init(void) {
  struct gdt_ptr gdt_ptr;

  gdt_ptr.base = (unsigned long)GDT;
  gdt_ptr.limit = sizeof(GDT);
  load_gdt(&gdt_ptr);
}
