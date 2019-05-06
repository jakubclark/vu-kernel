#include "memory/physmem.h"
#include "io/scrn.h"
#include "memory/memutil.h"
#include "std/types.h"

/* Total allocatable memory = PAGES * PAGE_SIZE */
uint32_t physmem_pages[PAGES];

void physmem_init() {
  for (uint32_t page = 0; page < phys_num_pages; page++) {
    physmem_pages[page] = 0xFFFFFFFF;
  }
}

int16_t set_bitmap(uint32_t start, uint32_t end, uint32_t pages,
                   uint8_t state) {
  uint32_t start_p, end_p;
  int16_t start_b, end_b;

  uint32_t p;
  int16_t b;

  uint32_t page_start = ATP(start);
  uint32_t page_end;

  if (pages != 0) {
    page_end = page_start + pages - 1;
  } else {
    page_end = ATP(end);
  }

  if (page_end > MEM_BLOCKS) {
    return MEM_ERR_RANGE;
  }

  if (page_start >= PAGE_BITS) {
    start_p = page_start / PAGE_BITS;
    start_b = page_start - (start_p * PAGE_BITS);
  } else {
    start_p = 0;
    start_b = page_start;
  }

  if (page_end >= PAGE_BITS) {
    end_p = page_end / PAGE_BITS;
    end_b = page_end - (end_p * PAGE_BITS);
  } else {
    end_p = 0;
    end_b = page_end;
  }

  if (end_p > start_p) {
    for (p = start_p; p <= end_p; p++) {
      if (p == end_p) {
        for (b = 0; b <= end_b; b++) {
          if (state == ALLOCATE) {
            set_bit(&physmem_pages[p], b);
          } else if (get_bit(physmem_pages[p], b) == FREE) {
            return MEM_ERR_DOUBLE_FREE;
          } else {
            clear_bit(&physmem_pages[p], b);
          }
        }
      } else if (p == start_p) {
        for (b = start_b; b < PAGE_BITS; b++) {
          if (state == ALLOCATE) {
            set_bit(&physmem_pages[p], b);
          } else {
            if (get_bit(physmem_pages[p], b) == FREE) {
              return MEM_ERR_DOUBLE_FREE;
            } else {
              clear_bit(&physmem_pages[p], b);
            }
          }
        }
      } else {
        for (b = 0; b < PAGE_BITS; b++) {
          if (state == ALLOCATE) {
            set_bit(&physmem_pages[p], b);
          } else if (get_bit(physmem_pages[p], b) == FREE) {
            return MEM_ERR_DOUBLE_FREE;
          } else {
            clear_bit(&physmem_pages[p], b);
          }
        }
      }
    }
  } else {
    for (b = start_b; b <= end_b; b++) {
      if (state == ALLOCATE) {
        set_bit(&physmem_pages[start_p], b);
      } else if (get_bit(physmem_pages[start_p], b) == FREE) {
        return MEM_ERR_DOUBLE_FREE;
      } else {
        clear_bit(&physmem_pages[start_p], b);
      }
    }
  }
  return NULL;
}

int16_t get_free(uint32_t size, uint32_t *start, uint32_t *end,
                 uint32_t *pages) {
  uint16_t b;
  uint32_t start_free, end_free, p;

  int8_t found_start = 0;
  uint32_t mempages, mempages_free = 0;

  start_free = NULL;

  if (size > PAGE_SIZE) {
    mempages = size / PAGE_SIZE;

    if (size % PAGE_SIZE != 0) {
      mempages++;
    }
  } else {
    mempages = 1;
  }
  *pages = mempages;

  for (p = KERN_PAGES / PAGE_BITS; p < phys_num_pages; p++) {
    for (b = 0; b < PAGE_BITS; b++) {
      if (get_bit(physmem_pages[p], b) == FREE) {
        if (found_start == 0) {
          found_start = 1;
          start_free = PAGE_BITS * PAGE_SIZE * p + b * PAGE_SIZE;
          mempages_free = 1;

          if (mempages == 1) {
            *start = start_free;
            *end = start_free;
            return NULL;
          }
        } else {
          mempages_free++;
          if (mempages_free >= mempages) {
            end_free = (PAGE_BITS * PAGE_SIZE * p + b * PAGE_SIZE) - 1;
            *start = start_free;
            *end = end_free;
            return NULL;
          }
        }
      } else if (mempages_free >= mempages) {
        end_free = (PAGE_BITS * PAGE_SIZE * p + b * PAGE_SIZE) - 1;

        *start = start_free;
        *end = end_free;
        return NULL;
      } else {
        found_start = 0;
        mempages_free = 0;
      }
    }
    if (mempages_free >= mempages) {
      end_free = (PAGE_BITS * PAGE_SIZE * p + b * PAGE_SIZE) - 1;

      *start = start_free;
      *end = end_free;
      return NULL;
    }
  }
  return MEM_ERR_NOMEM;
}

uint32_t count_free_pages(void) {
  uint16_t i, j, pages;
  pages = 0;
  for (i = 0; i < phys_num_pages / PAGE_BITS; i++) {
    for (j = 0; j < PAGE_BITS; j++) {
      if (get_bit(physmem_pages[i], j) == FREE) {
        pages++;
      }
    }
  }
  return pages;
}

void *kmalloc(uint32_t size) {
  uint32_t start, end, new_size, pages;
  uint32_t *ptr;
  new_size = size + sizeof(uint32_t);

  if (get_free(new_size, &start, &end, &pages) == NULL &&
      set_bitmap(start, end, pages, ALLOCATE) == NULL) {
    ptr = (uint32_t *)start;
    *ptr = pages;
    return (void *)(start + sizeof(uint32_t));
  }
  return NULL;
}

void kfree(uint32_t address) {
  uint32_t *ptr;
  uint32_t end_mem;
  uint32_t pages;

  ptr = (uint32_t *)(address - sizeof(uint32_t));
  pages = (uint32_t)*ptr;

  end_mem = (address - sizeof(uint32_t)) + pages * PAGE_SIZE;
  set_bitmap(address - sizeof(uint32_t), end_mem, pages, FREE);
}
