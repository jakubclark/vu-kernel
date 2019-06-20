#ifndef __ATA_h
#define __ATA_h

#include "std/types.h"

#define ATA_PRIMARY_DATA 0x1F0
#define ATA_PRIMARY_ERR 0x1F1
#define ATA_PRIMARY_SECTORS 0x1F2
#define ATA_PRIMARY_LBA_LOW 0x1F3
#define ATA_PRIMARY_LBA_MID 0x1F4
#define ATA_PRIMARY_LBA_HIGH 0x1F5
#define ATA_PRIMARY_DRIVE_SEL 0x1F6
#define ATA_PRIMARY_STATUS 0x1F7
#define ATA_PRIMARY_IRQ 14

#define ATA_SECONDARY_DATA 0x170
#define ATA_SECONDARY_ERR 0x171
#define ATA_SECONDARY_SECTORS 0x172
#define ATA_SECONDARY_LBA_LOW 0x173
#define ATA_SECONDARY_LBA_MID 0x174
#define ATA_SECONDARY_LBA_HIGH 0x175
#define ATA_SECONDARY_DRIVE_SEL 0x176
#define ATA_SECONDARY_STATUS 0x177
#define ATA_SECONDARY_IRQ 15

#define ATA_IDENTIFY 0xEC

typedef struct ata_drive {
  int present;
  int type;
  uint32_t data_reg;
  uint32_t err_reg;
  uint32_t sectors_reg;
  uint32_t lba_low_reg;
  uint32_t lba_mid_reg;
  uint32_t lba_high_reg;
  uint32_t sel_reg;
  uint32_t status_reg;
  uint32_t irq_num;
  int lba_mode;
  uint32_t total_sectors_28;
  uint64_t total_sectors_48;
} ata_drive_t;

typedef struct ata_drives {
  ata_drive_t cur_hdd;
  ata_drive_t primary_master;
  ata_drive_t primary_slave;
  ata_drive_t secondary_master;
  ata_drive_t secondary_slave;
} ata_drives_t;

uint16_t drive;

/* Defined in `boot.asm`. The interrupt handler */
extern void ata_int();

/* Intialize ATA driver. Mounts the HDD */
extern void init_ata();

/* Prepare to read sector number `sector` */
extern void read_sector(uint32_t sector);

/* Prepare to write to sector number `sector` */
extern void write_sector(uint32_t sector);

/* Read sector number `sector` and store the results into `buffer`.
 * The buffer itself is returned */
extern void ata_read_sector(uint32_t sector, uint8_t *buffer);

/* Write the contents of the buffer `buffer` at sector number `sector` */
extern uint32_t ata_write_sector(uint32_t sector, uint8_t *buffer);

/* Print sector number `sector` */
extern void print_sector(uint32_t sector);

#endif
