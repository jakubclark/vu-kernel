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
#include "routines.h"
#include "std/colors.h"
#include "std/types.h"
#include "fs/vfs.h"
#include "drivers/ata/ata.h"

void check_multiboot(uint32_t magic, multiboot_info_t *mbi) {
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    printf("MULTIBOOT_BOOTLOADER_MAGIC is incorrect: 0x%x\n", magic);
    PANIC((uint8_t *)"Multiboot Information is invalid-> "
                     "Multiboot_Bootloader_Magic is invalid");
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
  for (uint8_t i = 0; i < 80; i++)
    putchar('-');
  println("| VGA Done\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  |");

  gdt_init();
  println("| GDT Done\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  |");

  idt_init();
  println("| IDT Done\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  |");

  pmm_init();
  println("| PMM Done\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  |");

  check_multiboot(magic, mbi);
  println("| MBI Done\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  |");

  vmm_init(phys_num_pages);
  println("| VMM Done\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  |");

  kb_init();
  println("| KBD Done\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  |");

  vfs_init();
  println("| VFS Done\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  |");

  // init_ata();
  println("| ATA Done\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  |");

  uint8_t *buff = (uint8_t *)kmalloc(256);

  ata_read_sector(0, buff);

  for (int i = 0; i < 100; i += 2) {
    if (buff[i] < 0x10)
      printf("0%x", buff[i]);
    else
      printf("%x", buff[i]);

    if (buff[i + 1] < 0x10)
      printf("0%x ", buff[i + 1]);
    else
      printf("%x ", buff[i + 1]);

    if (((i+2) % 16) == 0 && i != 0)
      println("");
  }
  println("");

  for (uint8_t i = 0; i < 80; i++)
    putchar('-');

  set_default_color(MAGENTA, DEFAULTBACKGROUND);

  for (uint8_t i = 0; i < 80; i++)
    putchar('-');

  printf("| The system has %d bytes of physical memory\t\t\t\t\t\t\t  |\n",
  phys_mem_bytes);

  for (uint8_t i = 0; i < 80; i++)
    putchar('-');

  set_default_color(DEFAULTFOREGROUND, DEFAULTBACKGROUND);
}

void kernel_main(uint32_t magic, multiboot_info_t *mbi) {
  init(magic, mbi);
  chell_main();
  while (1)
    ;
}
