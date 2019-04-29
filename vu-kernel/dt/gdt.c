#include "dt/gdt.h"
#include "std/types.h"

gdt_ptr_t gdtptr;

gdt_entry_t GDT[GDT_SIZE] = {
    [GDT_NULL] = {0 /* NULL GDT entry - Required */},
    [KERNEL_CS] = GDT_ENTRY(GDT_EXE | GDT_READ, 0, 0xFFFFFFFF, DPL_KERNEL),
    [KERNEL_DS] = GDT_ENTRY(GDT_WRITE, 0, 0xFFFFFFFF, DPL_KERNEL)};

void gdt_init(void) {
  gdtptr.base = (unsigned long)GDT;
  gdtptr.limit = sizeof(GDT);
  load_gdt();
}
