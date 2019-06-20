#ifndef __ATA_h
#define __ATA_h

#include "std/types.h"


uint16_t drive;

/* Intialize ATA driver. Mounts the HDD */
extern void init_ata();

/* Read sector number `addr` and store the results into `buffer`.
 * The buffer itself is returned */
extern void ata_read_sector(uint32_t addr, uint8_t *buffer);

/* Write the contents of the buffer `buffer` at sector number `addr` */
extern uint32_t ata_write_sector(uint32_t addr, uint8_t *buffer);

#endif
