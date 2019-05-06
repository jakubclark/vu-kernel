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

void init() {
  vga_init();
  puts_col((uint8_t *)"VGA Done\n", CYAN, DEFAULTBACKGROUND);

  gdt_init();
  puts_col((uint8_t *)"GDT Done\n", CYAN, DEFAULTBACKGROUND);

  idt_init();
  puts_col((uint8_t *)"IDT Done\n", CYAN, DEFAULTBACKGROUND);

  physmem_init();
  puts_col((uint8_t *)"PMM Done\n", CYAN, DEFAULTBACKGROUND);

  kb_init();
  puts_col((uint8_t *)"KBD Done\n", CYAN, DEFAULTBACKGROUND);
}

void kernel_main(uint32_t magic, multiboot_info_t *mbi) {
  init();
  check_multiboot(magic, mbi);
  chell_main();
  while (1)
    ;
}
