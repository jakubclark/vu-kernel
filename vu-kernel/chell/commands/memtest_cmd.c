#include "io/keyboard.h"
#include "io/scrn.h"
#include "memory/memutil.h"
#include "memory/physmem.h"
#include "std/colors.h"
#include "std/types.h"

uint32_t memsize;

uint32_t *page_alloc(uint32_t n) { return (uint32_t *) kmalloc(n * PAGE_SIZE); }

void page_free(uint32_t *addr) { kfree((uint32_t)addr); }

void assert(int b) {
  if (b)
    return;

  printf("ASSERTION ERROR! HALTING THE SYSTEM. b: %d", b);
  while (1)
    ;
}

void assert_page_contents(char *p, char c, uint32_t j) {
  for (uint32_t i = 0; i < PAGE_SIZE; i++) {
    if (p[i] != c) {
      printf("Expected to find `%d`, found `%d` instead. page[%d], char[%d]", c,
             p[i], j, i);
      while (1) {
      };
    }
  }
}

void init_mem(uint32_t sz) { memsize = sz; }

void test_alloc_easy(uint32_t verbose) {
  uint32_t npages = memsize / PAGE_SIZE;
  uint32_t nsample = npages - 1 < PAGE_SIZE / sizeof(void *)
                         ? npages - 1
                         : PAGE_SIZE / sizeof(void *);

  uint32_t **p = (uint32_t **)page_alloc(1);

  uint32_t pages_to_alloc = nsample / 3;

  if (verbose)
    printf(" EASY-Attempting to allocate %d pages\n", pages_to_alloc);

  for (uint32_t i = 0; i < pages_to_alloc; i++) {
    p[i] = (uint32_t *)page_alloc(1);
    assert(p[i] != NULL);

    if (i % 200 == 0)
      puts_col((uint8_t *)".", RED, DEFAULTBACKGROUND);
  }

  if (verbose)
    printf(" EASY-Successfully allocated %d pages\n", pages_to_alloc);

  if (verbose)
    printf(" EASY-Attempting to free %d pages\n", pages_to_alloc);

  for (uint32_t i = 0; i < pages_to_alloc; i++) {
    page_free(p[i]);

    if (i % 200 == 0)
      puts_col((uint8_t *)".", RED, DEFAULTBACKGROUND);
  }
  println("");

  if (verbose)
    printf(" EASY-Successfully freed %d pages\n", pages_to_alloc);
}

void test_alloc_advanced(uint32_t verbose) {
  uint32_t npages = memsize / PAGE_SIZE;
  assert(npages > 0);

  uint32_t **p = (uint32_t **)page_alloc(1);

  uint32_t nsample = npages - 1 < PAGE_SIZE / sizeof(void *)
                         ? npages - 1
                         : PAGE_SIZE / sizeof(void *);

  if (verbose)
    printf(" HARD-Attempting to allocate %d pages\n", nsample);

  for (uint32_t i = 0; i < nsample; i++) {
    p[i] = (uint32_t *)page_alloc(1);
    assert(p[i] != NULL);

    if (i % 200 == 0)
      puts_col((uint8_t *)".", RED, DEFAULTBACKGROUND);
  }

  if (verbose)
    printf(" HARD-Successfully allocated %d pages\n", nsample);

  if (verbose)
    printf(" HARD-Attempting to write to %d pages\n", nsample);

  for (uint32_t i = 0; i < nsample; i++) {
    char *ptr = (char *)p[i];
    char v = 0x42 + i % 256;
    memset((uint8_t *)ptr, v, PAGE_SIZE);
    assert_page_contents(ptr, v, i);

    if (i % 200 == 0)
      puts_col((uint8_t *)".", RED, DEFAULTBACKGROUND);
  }

  if (verbose)
    printf(" HARD-Successfully wrote to  %d pages\n", nsample);

  if (verbose)
    printf(" HARD-Attempting to free     %d  pages - even \n", nsample / 2);

  for (uint32_t i = 0; i < nsample; i += 2) {
    char v = 0x42 + i % 256;
    assert_page_contents((char *)p[i], v, i);
    page_free(p[i]);

    if (i % 200 == 0)
      puts_col((uint8_t *)".", RED, DEFAULTBACKGROUND);
  }

  if (verbose)
    printf(" HARD-Successfully freed     %d  pages - even\n", nsample / 2);

  if (verbose)
    printf(" HARD-Attempting to free     %d  pages - odd \n", nsample / 2);

  for (uint32_t i = 1; i < nsample; i += 2) {
    char v = 0x42 + i % 256;
    assert_page_contents((char *)p[i], v, i);
    page_free(p[i]);

    if (i % 200 == 0)
      puts_col((uint8_t *)".", RED, DEFAULTBACKGROUND);
  }
  println("");

  if (verbose)
    printf(" HARD-Successfully freed     %d  pages - odd\n", nsample / 2);
}

void test_alloc_oom(uint32_t verbose) {
  uint32_t allocated = 0;
  uint32_t npages = memsize / PAGE_SIZE;
  uint32_t nsample = npages - KERN_PAGES;

  uint32_t init_npages = (npages * sizeof(uint32_t)) / PAGE_SIZE;

  if (verbose)
    printf("  OOM-init-Attempting to allocate %d pages=\n", init_npages);

  uint32_t **p = (uint32_t **)page_alloc(init_npages);

  if (verbose)
    printf("  OOM-init-Successfully allocated %d pages\n", init_npages);

  if (verbose)
    printf("  OOM-Attempting to allocate %d pages\n", nsample);

  for (uint32_t i = 0; i < nsample; i++) {
    p[i] = (uint32_t *)page_alloc(1);
    if (p[i]) {
      memset((uint8_t *)p[i], 0x42, PAGE_SIZE);
      allocated++;
      if(i % 200 == 0)
        puts_col((uint8_t *)".", RED, DEFAULTBACKGROUND);
    }
  }

  if (verbose)
    printf("  OOM-Successfully allocated %d pages\n", nsample);

  assert(allocated >= npages - KERN_PAGES);

  if (verbose)
    printf("  OOM-Attempting to free     %d pages\n", nsample);

  for (uint32_t i = 0; i < nsample; i++) {
    if (p[i]) {
      page_free(p[i]);
      if (i % 200 == 0)
        puts_col((uint8_t *)".", RED, DEFAULTBACKGROUND);
    }
  }
  println("");

  if (verbose)
    printf("  OOM-Successfully freed     %d pages\n", nsample);
}
