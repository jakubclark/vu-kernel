#include "chell.h"
#include "colors.h"
#include "multiboot.h"
#include "scrn.h"
#include "types.h"
#include "keyboard.h"
#include "idt.h"
#include "gdt.h"

#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))

extern uint32_t mem_start_address;
extern uint32_t mem_end_address;
extern uint32_t mem_use_address;

/* Memory map struct */
typedef struct multiboot_memory_map {
  uint32_t size;
  uint64_t base_addr;
  uint64_t length;
  uint32_t type;
} multiboot_memory_map_t;

void print_multiboot_info(uint32_t magic, multiboot_info_t *mbi) {
  if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
    printf("Multiboot magic number is correct: %x\n", magic);
  } else {
    printf("Multiboot magic number is incorrect: %x\n", magic, mbi);
  }

  printf("flags: 0x%x\n", mbi->flags);

  if (CHECK_FLAG(mbi->flags, 0)) {
    printf("mem_lower: %uKB, mem_upper: %uKB\n", mbi->mem_lower,
           mbi->mem_upper);
  }

  if (CHECK_FLAG(mbi->flags, 1)) {
    printf("boot_device: 0x%x\n", mbi->boot_device);
  }

  if (CHECK_FLAG(mbi->flags, 2)) {
    printf("cmdline: 0x%x\n", mbi->cmdline);
  }

  if (CHECK_FLAG(mbi->flags, 3)) {
    multiboot_module_t *mod;
    int i;
    printf("mods_count: %d, mods_addr: 0x%x\n", mbi->mods_count,
           mbi->mods_addr);

    for (i = 0, mod = (multiboot_module_t *)mbi->mods_addr;
         i < (int)mbi->mods_count; i++, mod++) {
      printf("mods_start: 0x%x, mods_end: 0x%x\n", mod->mod_start,
             mod->mod_end);
    }
  }

  if (CHECK_FLAG(mbi->flags, 4) && CHECK_FLAG(mbi->flags, 5)) {
    printf("Both bits 4 and 5 are set.\n");
    return;
  }

  if (CHECK_FLAG(mbi->flags, 4)) {
    multiboot_aout_symbol_table_t *aout_sym = &(mbi->u.aout_sym);

    printf(
        "aout_symbol_table: tabsize: 0x%0x, "
        "strsize = 0x%x`, addr: 0x%x\n",
        aout_sym->tabsize, aout_sym->strsize, aout_sym->addr);
  }

  if (CHECK_FLAG(mbi->flags, 5)) {
    multiboot_elf_section_header_table_t *elf_sec = &(mbi->u.elf_sec);
    printf(
        "elf_sec: num: %u, size: 0x%x, "
        "addr: 0x%x, shndx: 0x%x\n",
        elf_sec->num, elf_sec->size, elf_sec->addr, elf_sec->shndx);
  }

  if (CHECK_FLAG(mbi->flags, 6)) {
    multiboot_memory_map_t *mmap;
    uint32_t mem_start, mem_end, mem_type;

    printf("mmap_addr = 0x%x, mmap_length = 0x%x\n", (uint32_t)mbi->mmap_addr,
           (uint32_t)mbi->mmap_length);

    for (mmap = (multiboot_memory_map_t *)mbi->mmap_addr;
         (unsigned long)mmap < mbi->mmap_addr + mbi->mmap_length;
         mmap = (multiboot_memory_map_t *)((unsigned long)mmap + mmap->size +
                                           sizeof(mmap->size))) {
      mem_start = mmap->base_addr;
      mem_end = mem_start + mmap->length;
      mem_type = mmap->type;

      if (mem_type == 1) {
        printf("addr: 0x%x ", (uint32_t)mmap->base_addr);

        if (mmap->length < 1024 * 1024)
          printf("size: %d bytes ", (uint32_t)mmap->length);
        else
          printf("size: %dMB, ", (uint32_t)mmap->length / (1024 * 1024));

        printf("type: 0x%x, end: 0x%x\n", mmap->type, mem_end);
      }
    }
    println("");
  }
}

void print_colored_text() {
  puts((uint8_t *)"Hello world!\nThis is the second line!\tTab\n");
  puts((uint8_t *)"Very Long Chain of Chars\rCarriage return\n");

  puts_col((uint8_t *)"Black on blue\n", BLACK, BLUE);
  puts_col((uint8_t *)"Green on cyan\n", GREEN, CYAN);
  puts_col((uint8_t *)"Red on magenta\n", RED, MAGENTA);
  puts_col((uint8_t *)"Brown on light gray\n", BROWN, LIGHTGRAY);
  puts_col((uint8_t *)"Dark gray on light blue\n", DARKGRAY, LIGHTBLUE);
  puts_col((uint8_t *)"Light green on ligt cyan\n", LIGHTGREEN, LIGHTCYAN);
  puts_col((uint8_t *)"Light red on pink\n", LIGHTRED, PINK);
  puts_col((uint8_t *)"Yellow on white\n", YELLOW, WHITE);

  printf("Formatted string: str_arg: `%s`, int_arg: `%d`, int_arg: `%d`",
         "ArgumentA", 59, -12);
}

void init() {
  vga_init();
  gdt_init();
  idt_init();
  kb_init();
}

void kernel_main(uint32_t magic, multiboot_info_t *mbi) {
  init();

  print_multiboot_info(magic, mbi);
  // print_colored_text();

  chell_main();

  while (1)
    ;
}
