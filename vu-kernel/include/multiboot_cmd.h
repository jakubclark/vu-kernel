#ifndef __MULTIBOOT_CMD_H
#define __MULTIBOOT_CMD_H

#include "multiboot.h"
#include "types.h"

/* Set the magic number and the address of `multiboot_info_t` */
extern void set_addrs(uint32_t magic_, multiboot_info_t *mbi_);

extern void print_multiboot_info();

#endif
