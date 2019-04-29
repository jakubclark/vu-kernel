#include "kernel.h"
#include "chell/chell.h"
#include "chell/commands/memtest_cmd.h"
#include "chell/commands/multiboot_cmd.h"
#include "dt/gdt.h"
#include "dt/idt.h"
#include "io/keyboard.h"
#include "io/scrn.h"
#include "memory/memutil.h"
#include "memory/physmem.h"
#include "multiboot.h"
#include "std/types.h"

void check_multiboot(uint32_t magic, multiboot_info_t *mbi) {
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    printf("MULTIBOOT_BOOTLADER_MAGIC is incorrect: 0x%x\n", magic);
    // TODO: PANIC
    while (1)
      ;
  }

  phys_mem_bytes = 0;

  if (CHECK_FLAG(mbi->flags, 2)) {
    multiboot_memory_map_t *mmap;
    for (mmap = (multiboot_memory_map_t *)mbi->mmap_addr;
         (unsigned long)mmap < mbi->mmap_addr + mbi->mmap_length;
         mmap = (multiboot_memory_map_t *)((unsigned long)mmap + mmap->size +
                                           sizeof(mmap->size))) {

      if (mmap->type == 1)
        phys_mem_bytes += mmap->length;
    }
  }

  set_addrs(magic, mbi);

  init_mem(phys_mem_bytes);
}

void init() {
  vga_init();
  println("VGA Done");

  // pmem_init_bitmap();
  // initialise_paging();
  // println("PAG Done");

  gdt_init();
  println("GDT Done");

  idt_init();
  println("IDT Done");

  pmem_init_bitmap();

  kb_init();
  println("KBD Done");
}

void kernel_main(uint32_t magic, multiboot_info_t *mbi) {
  init();
  check_multiboot(magic, mbi);

  uint32_t theory_free_pages = phys_mem_bytes / PAGE_SIZE;
  phys_num_pages = theory_free_pages;
  printf("phys_mem_bytes=%d\n", phys_mem_bytes);
  printf("phys_num_pages=%d\n", phys_num_pages);

  chell_main();
  while (1)
    ;
}
