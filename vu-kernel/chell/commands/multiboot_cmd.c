#include "kernel.h"
#include "multiboot.h"
#include "scrn.h"
#include "types.h"

uint32_t magic;
multiboot_info_t *mbi;

#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))

void set_addrs(uint32_t magic_, multiboot_info_t *mbi_) {
  magic = magic_;
  mbi = mbi_;
}

void print_multiboot_info() {
  if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
    printf("Multiboot magic number is correct: 0x%x\n", magic);
  } else {
    printf("Multiboot magic number is incorrect: 0x%x\n", magic);
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
        printf(" addr: 0x%x,", (uint32_t)mmap->base_addr);

        if (mmap->length < 1024 * 1024)
          printf(" size: %d bytes,", (uint32_t)mmap->length);
        else
          printf(" size: %dMB,", (uint32_t)mmap->length / (1024 * 1024));

        printf(" type: 0x%x, end: 0x%x\n", mmap->type, mem_end);
      }
    }
  }
  printf("bootloader name: %s", mbi->boot_loader_name);
}
