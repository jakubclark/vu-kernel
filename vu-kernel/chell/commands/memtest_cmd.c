#include "io/keyboard.h"
#include "io/scrn.h"
#include "memory/physmem.h"
#include "std/types.h"

uint32_t memsize;

uint32_t *page_alloc(uint32_t n) { return kmalloc(n * PAGE_SIZE); }

void page_free(uint32_t *addr) { kfree((uint32_t)addr); }

void assert(int b) {
  if (b)
    return;

  printf("ASSERTION ERROR! HALTING THE SYSTEM. b: %d", b);
  while (1)
    ;

  /* Cause a breakpoint exception. */
  while (1) {
    asm volatile("int3");
  }
}

void assert_page_contents(char *p, char c, uint32_t j) {
  for (uint32_t i = 0; i < PAGE_SIZE; i++) {
    if (p[i] == c) {
      continue;
    }
    printf("Expected to find `%d`, found `%d` instead. page[%d], char[%d]", c,
           p[i], j, i);
    while (1)
      ;
  }
}

void *memset_(void *s, int c, size_t n) {
  unsigned char *p = s;
  while (n--)
    *p++ = (unsigned char)c;
  return s;
}

void init_mem(uint32_t sz) { memsize = sz; }

void test_alloc_easy() {
  uint32_t npages = memsize / PAGE_SIZE;
  uint32_t nsample = npages - 1 < PAGE_SIZE / sizeof(void *)
                         ? npages - 1
                         : PAGE_SIZE / sizeof(void *);

  uint32_t **p = (uint32_t **)page_alloc(1);

  uint32_t pages_to_alloc = nsample / 3;

  printf("EASY-Attempting to allocate %d pages\n", pages_to_alloc);
  for (uint32_t i = 0; i < pages_to_alloc; i++) {
    p[i] = (uint32_t *)page_alloc(1);
    assert(p[i] != NULL);
  }
  printf("EASY-Successfully allocated %d pages\n", pages_to_alloc);

  printf("EASY-Attempting to free %d pages\n", pages_to_alloc);
  for (uint32_t i = 0; i < pages_to_alloc; i++) {
    page_free(p[i]);
  }
  printf("EASY-Successfully freed %d pages\n", pages_to_alloc);
}

void test_alloc_advanced() {
  uint32_t npages = memsize / PAGE_SIZE;
  assert(npages > 0);

  // Keep track of allocated pages
  uint32_t **p = (uint32_t **)page_alloc(1);

  uint32_t nsample = npages - 1 < PAGE_SIZE / sizeof(void *)
                         ? npages - 1
                         : PAGE_SIZE / sizeof(void *);

  // printf("HARD-npages=%d, nsample=%d\n", npages, nsample);

  printf("HARD-Attempting to allocate %d pages\n", nsample);
  // Allocate one page worth of pointers
  for (uint32_t i = 0; i < nsample; i++) {
    p[i] = (uint32_t *)page_alloc(1);
    assert(p[i] != NULL);
  }
  printf("HARD-Successfully allocated %d pages\n", nsample);
  // while(1);

  printf("HARD-Attempting to write to %d pages\n", nsample);
  for (uint32_t i = 0; i < nsample; i++) {
    char *ptr = (char *)p[i];
    char v = 0x42 + i % 256;
    memset_(ptr, v, PAGE_SIZE);
    // assert_page_contents(ptr, v, i);
  }
  printf("HARD-Successfully wrote to  %d pages\n", nsample);

  // Free even pages
  printf("HARD-Attempting to free     %d  pages - even \n", nsample / 2);
  for (uint32_t i = 0; i < nsample; i += 2) {
    char v = 0x42 + i % 256;
    // if(i == 32) continue;
    assert_page_contents((char *)p[i], v, i);
    page_free(p[i]);
  }
  printf("HARD-Successfully freed     %d  pages - even\n", nsample / 2);

  // Free odd pages
  printf("HARD-Attempting to free     %d  pages - odd \n", nsample / 2);
  for (uint32_t i = 1; i < nsample; i += 2) {
    char v = 0x42 + i % 256;
    assert_page_contents((char *)p[i], v, i);
    page_free(p[i]);
  }
  printf("HARD-Successfully freed     %d  pages - odd\n", nsample / 2);
}

void test_alloc_oom() {
  uint32_t npages = memsize / PAGE_SIZE;
  uint32_t nsample = npages - KERN_PAGES;

  // Keep track of allocated pages
  uint32_t **p =
      (uint32_t **)page_alloc((npages * sizeof(uint32_t)) / PAGE_SIZE);

  printf("Attempting to allocate %d pages\n", nsample);

  // Allocate all the pages
  uint32_t allocated = 0;
  for (uint32_t i = 0; i < nsample; i++) {
    p[i] = (uint32_t *)page_alloc(1);
    if (p[i]) {
      memset_(p[i], 0x42, PAGE_SIZE);
      allocated++;
    }
  }

  printf("Successfully allocated %d pages = %d bytes\n", allocated,
         allocated * 4096);
  // print: allocated `allocated` pages
  assert(allocated >= npages - KERN_PAGES);

  // Free all allocated pages
  for (uint32_t i = 0; i < nsample; i++) {
    if (p[i]) {
      page_free(p[i]);
    }
  }
  printf("Successfully freed %d pages\n", nsample);
}

void test_all() {
  println("Starting mem tests");
  // Memory alloc
  test_alloc_easy();
  // test_alloc_advanced();
  // test_alloc_oom();
}
