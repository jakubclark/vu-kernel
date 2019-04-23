#include "types.h"

uint32_t page_dir[1024] __attribute__((aligned(4096)));
uint32_t first_page[1024] __attribute__((aligned(4096)));

void init_vmem(void) {
  // Initialize the page directory and the first page
  for (int i = 0; i < 1024; i++) {
    first_page[i] = (i * 0x1000) | 3;
    page_dir[i] = 0x00000002;
  }
  // loadPageDirectory(page_dir);
  // enablePaging();
}
