#include "memory/physmem.h"
#include "io/scrn.h"
#include "memory/memutil.h"
#include "std/types.h"

/* Total allocatable memory = PAGES * PAGE_SIZE */
uint32_t physmem_pages[PAGES];

void set_bit_(uint32_t *ptr, uint16_t bit) {
  uint32_t ret = *ptr;

  ret |= 1 << bit;

  *ptr = ret;
}

void clear_bit_(uint32_t *ptr, uint16_t bit) {
  uint32_t ret = *ptr;

  ret &= ~(1 << bit);

  *ptr = ret;
}

uint16_t get_bit_(uint32_t n, uint16_t bit) {
  uint16_t bitset;

  bitset = (n >> bit) & 1;
  return (bitset);
}

void pmem_init_bitmap() {
  for (uint32_t page = 0; page < phys_num_pages; page++) {
    /* Every page is free initially */
    physmem_pages[page] = 0xFFFFFFFF;
  }
  //   set_bit(&physmem_pages[0], FREE);
}

uint32_t pmem_get_page_from_address(uint32_t address) {
  //   uint32_t page;
  return address / MEM_BLOCK_SIZE;
  //   return (page);
}

int16_t pmem_set_bit_map(uint32_t start, uint32_t end, uint32_t pages,
                         uint8_t state) {
  /* set bits in bitmap  */
  uint32_t start_p, end_p;
  int16_t start_b, end_b;

  uint32_t p;
  int16_t b;

  uint32_t page_start = pmem_get_page_from_address(start);
  uint32_t page_end;

  if (pages != 0) {
    page_end = page_start + pages - 1;
  } else {
    page_end = pmem_get_page_from_address(end);
  }

  if (page_end > MEM_BLOCKS) {
    return (MEM_ERR_RANGE);
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
          } else {
            if (get_bit(physmem_pages[p], b) == FREE) {
              return (MEM_ERR_DOUBLE_FREE);
            } else {
              clear_bit(&physmem_pages[p], b);
            }
          }
        }
      } else {
        if (p == start_p) {
          for (b = start_b; b < PAGE_BITS; b++) {
            if (state == ALLOCATE) {
              set_bit(&physmem_pages[p], b);
            } else {
              if (get_bit(physmem_pages[p], b) == FREE) {
                return (MEM_ERR_DOUBLE_FREE);
              } else {
                clear_bit(&physmem_pages[p], b);
              }
            }
          }
        } else {
          for (b = 0; b < PAGE_BITS; b++) {
            if (state == ALLOCATE) {
              set_bit(&physmem_pages[p], b);
            } else {
              if (get_bit(physmem_pages[p], b) == FREE) {
                return (MEM_ERR_DOUBLE_FREE);
              } else {
                clear_bit(&physmem_pages[p], b);
              }
            }
          }
        }
      }
    }
  } else {
    for (b = start_b; b <= end_b; b++) {
      if (state == ALLOCATE) {
        set_bit(&physmem_pages[start_p], b);
      } else {
        if (get_bit(physmem_pages[start_p], b) == FREE) {
          return (MEM_ERR_DOUBLE_FREE);
        } else {
          clear_bit(&physmem_pages[start_p], b);
        }
      }
    }
  }
  return (MEM_ERR_OK);
}

int16_t pmem_get_free(size_t size, uint32_t *start, uint32_t *end,
                      uint32_t *pages) {
  /* return pointers to free memory range */
  int8_t found_start = 0;

  uint32_t p;
  uint16_t b;
  uint32_t start_free, end_free;
  size_t mempages, mempages_free = 0;
  start_free = NULL;

  if (size > MEM_BLOCK_SIZE) {
    mempages = size / MEM_BLOCK_SIZE;

    if (size % MEM_BLOCK_SIZE != 0) {
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
          start_free = PAGE_BITS * MEM_BLOCK_SIZE * p + b * MEM_BLOCK_SIZE;
          mempages_free = 1;

          if (mempages == 1) {
            *start = start_free;
            *end = start_free;
            return (MEM_ERR_OK);
          }
        } else {
          mempages_free++;
          if (mempages_free >= mempages) {
            end_free =
                (PAGE_BITS * MEM_BLOCK_SIZE * p + b * MEM_BLOCK_SIZE) - 1;
            *start = start_free;
            *end = end_free;
            return (MEM_ERR_OK);
          }
        }
      } else {
        if (mempages_free >= mempages) {
          end_free = (PAGE_BITS * MEM_BLOCK_SIZE * p + b * MEM_BLOCK_SIZE) -
          1;

          *start = start_free;
          *end = end_free;
          return (MEM_ERR_OK);
        } else {
          found_start = 0;
          mempages_free = 0;
        }
      }
    }
    if (mempages_free >= mempages) {
      end_free = (PAGE_BITS * MEM_BLOCK_SIZE * p + b * MEM_BLOCK_SIZE) - 1;

      *start = start_free;
      *end = end_free;
      return (MEM_ERR_OK);
    }
  }
  return (MEM_ERR_NOMEM);
}

uint32_t pmem_count_free_pages(void) {
  uint32_t pages = 0;
  uint32_t p;
  uint16_t b;

  for (p = 0; p < phys_num_pages / PAGE_BITS; p++) {
    for (b = 0; b < PAGE_BITS; b++) {
      if (get_bit_(physmem_pages[p], b) == FREE) {
        pages++;
      }
    }
  }
  return (pages);
}

void *kmalloc(size_t size) {
  uint32_t start_free, end_free;
  uint32_t *ptr;
  size_t new_size;
  uint32_t pages;
  new_size = size + sizeof(size_t);

  if (pmem_get_free(new_size, &start_free, &end_free, &pages) == MEM_ERR_OK) {

    if (pmem_set_bit_map(start_free, end_free, pages, ALLOCATE) == MEM_ERR_OK) {
      ptr = (uint32_t *)start_free;
      *ptr = pages;
      return (void *)(start_free + sizeof(size_t));
    } else {
      return (NULL);
    }
  } else {
    return (NULL);
  }
}

uint32_t kfree(uint32_t address) {
  uint32_t *ptr;
  uint32_t end_mem;
  size_t pages;

  ptr = (uint32_t *)(address - sizeof(size_t));
  pages = (size_t)*ptr;

  end_mem = (address - sizeof(size_t)) + pages * MEM_BLOCK_SIZE;

  if (pmem_set_bit_map(address - sizeof(size_t), end_mem, pages, FREE) ==
      MEM_ERR_OK) {
    return (NULL);
  } else {
    return (MEM_ERR_BAD_MEM);
  }
}

uint32_t pmem_get_page(uint32_t n) { return physmem_pages[n]; }
