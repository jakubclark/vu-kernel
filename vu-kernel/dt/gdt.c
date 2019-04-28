#include "dt/gdt.h"
#include "std/types.h"

struct gdt_entry gdt[3];
struct gdt_ptr gdtptr;

struct gdt_entry GDT[GDT_SIZE] = {
    [GDT_NULL] = {0 /* NULL GDT entry - Required */},
    [KERNEL_CS] = GDT_ENTRY(GDT_EXE | GDT_READ, 0, 0xFFFFFFFF, DPL_KERNEL),
    [KERNEL_DS] = GDT_ENTRY(GDT_WRITE, 0, 0xFFFFFFFF, DPL_KERNEL)};

void gdt_init(void) {
  gdtptr.base = (unsigned long)GDT;
  gdtptr.limit = sizeof(GDT);
  load_gdt();
}
