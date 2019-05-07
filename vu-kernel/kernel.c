#include "kernel.h"
#include "chell/chell.h"
#include "chell/commands/memtest_cmd.h"
#include "chell/commands/multiboot_cmd.h"
#include "dt/gdt.h"
#include "dt/idt.h"
#include "io/keyboard.h"
#include "io/scrn.h"
#include "memory/memutil.h"
#include "memory/paging.h"
#include "memory/physmem.h"
#include "multiboot.h"
#include "std/colors.h"
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

  phys_num_pages = phys_mem_bytes / PAGE_SIZE;

  set_addrs(magic, mbi);
  init_mem(phys_mem_bytes);
}

void init(uint32_t magic, multiboot_info_t *mbi) {
  set_default_color(CYAN, DEFAULTBACKGROUND);

  vga_init();
  println("VGA Done");

  gdt_init();
  println("GDT Done");

  idt_init();
  println("IDT Done");

  physmem_init();
  println("PMM Done");

  check_multiboot(magic, mbi);
  println("MBI Done");

  initialize_paging(phys_num_pages);
  println("VMM Done");

  kb_init();
  println("KBD Done");

  for (uint8_t i = 0; i < 80; i++) {
    putchar('-');
  }

  set_default_color(DEFAULTFOREGROUND, DEFAULTBACKGROUND);
}

void kernel_main(uint32_t magic, multiboot_info_t *mbi) {
  init(magic, mbi);
  chell_main();
  while (1)
    ;
}
