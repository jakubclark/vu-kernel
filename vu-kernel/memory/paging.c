#include "memory/paging.h"
#include "io/scrn.h"
#include "memory/memutil.h"
#include "memory/physmem.h"
#include "routines.h"
#include "std/types.h"

extern void load_page_directory(uint32_t);
extern void enable_paging();

uint32_t kernel_directory[1024] __attribute__((aligned(4096)));

extern uint32_t kernel_top;

/* This is where we start creating our page_directory
 * The page directory and all the page tables go here,
 * up to 0x400000 bytes up. */
uint32_t placement_address;

/* Return physical address of the next page/frame */
uint32_t palloc() {
  uint32_t tmp = placement_address;
  placement_address += PAGE_SIZE;
  return tmp;
}

void vmm_init(uint32_t phys_num_pages) {
  placement_address = PAGE_ALIGN((uint32_t)&kernel_top);

  uint32_t num_dir_entries_needed = phys_num_pages / 1024 + 1;

  uint32_t i;
  for (i = 0; i < 1024; i++) {
    kernel_directory[i] = READWRITE;
  }

  uint32_t *table;

  uint32_t dir_entry;
  uint32_t paddr;
  dir_entry = 0;
  for (dir_entry = 0; dir_entry < num_dir_entries_needed; dir_entry++) {
    table = (uint32_t *)palloc();
    for (i = 0; i < 1024; i++) {
      if (dir_entry * i >= phys_num_pages)
        break;
      paddr = (1024 * dir_entry * PAGE_SIZE) + (i * PAGE_SIZE);
      table[i] = paddr | PRESENT | READWRITE;
    }
    kernel_directory[dir_entry] = (uint32_t)table | PRESENT | READWRITE;
  }

  load_page_directory((uint32_t)&kernel_directory);
  enable_paging();
}

void page_fault(registers_t regs) {
  uint32_t faulting_address;
  uint8_t err = 0;
  uint8_t *err_str;
  asm volatile("mov %%cr2, %0" : "=r"(faulting_address));

  int p = !(regs.err_code & 0x1);
  int rw = regs.err_code & 0x2;
  int us = regs.err_code & 0x4;
  int reserved = regs.err_code & 0x8;

  printf("\nPage fault! ( ");
  if (p) {
    printf("present ");
    err &= 1;
  }
  if (rw) {
    printf("read-only ");
    err &= 2;
  }
  if (us) {
    printf("user-mode ");
    err &= 4;
  }
  if (reserved) {
    printf("reserved ");
  }

  printf(") at 0x%x\n", faulting_address);

  switch (err) {
  case 0:
    err_str =
        (uint8_t *)"Supervisory process tried to read a non-present page entry\n";
    break;

  case 1:
    err_str =
        (uint8_t *)"Supervisory process tried to read a page and caused a "
                   "protection fault\n";
    break;

  case 2:
    err_str =
        (uint8_t
             *)"Supervisory process tried to write to a non-present page entry\n";
    break;

  case 3:
    err_str =
        (uint8_t *)"Supervisory process tried to write a page and caused a "
                   "protection fault\n";
    break;

  case 4:
    err_str = (uint8_t *)"User process tried to read a non-present page entry\n";
    break;

  case 5:
    err_str = (uint8_t *)" User process tried to read a page and caused a "
                         "protection fault\n";
    break;

  case 6:
    err_str =
        (uint8_t *)"User process tried to write to a non-present page entry\n";
    break;

  case 7:
    err_str = (uint8_t *)"User process tried to write a page and caused a "
                         "protection fault\n";
    break;

  default:
    err_str = (uint8_t *)"Unkown error type\n";
  }

  puts(err_str);
  PANIC((uint8_t *) "");
}
