#include "device.h"
#include "drivers/ata/ata.h"
#include "fs/fat.h"
#include "fs/vfs.h"
#include "io/basicio.h"
#include "io/scrn.h"
#include "memory/physmem.h"
#include "std/types.h"

device_t ata_dev_info;

/* Used in the IRQ Handler */
uint8_t ata_irq_done = 0;

/* Prepare the drive to reador write */
void prepare_drive(uint32_t sector, uint32_t write) {
  outbyte(0x1F1, 0x00);
  outbyte(0x1F2, 0x01);
  outbyte(0x1F3, (uint8_t)sector);
  outbyte(0x1F4, (uint8_t)(sector >> 8));
  outbyte(0x1F5, (uint8_t)(sector >> 16));
  outbyte(0x1F6, 0xE0 | (drive << 4) | ((sector >> 24) & 0x0F));
  if (write)
    outbyte(0x1F7, 0x30);
  else
    outbyte(0x1F7, 0x20);
}

void init_ata() {
  // Mount the drive
  ata_dev_info.id = 0;
  ata_dev_info.type = 1;
  ata_dev_info.mount[0] = 'h';
  ata_dev_info.mount[1] = 'd';
  ata_dev_info.mount[2] = 'a';
  ata_dev_info.mount[3] = 0;
  ata_dev_info.read = &ata_read_sector;
  ata_dev_info.write = &ata_write_sector;

  fat_init(&ata_dev_info.fs);
  device_register(&ata_dev_info);

  drive = 0;
}

void ata_read_sector(uint32_t sector, uint8_t *buffer) {
  prepare_drive(sector, 0);

  while (!(inbyte(0x1F7) & 0x08))
    ;

  for (int idx = 0; idx < 256; idx++)
    *(uint32_t *)(buffer + idx * 4) = inword(0x1F0);
}

uint32_t ata_write_sector(uint32_t sector, uint8_t *buffer) {
  prepare_drive(sector, 1);

  while (!(inbyte(0x1F7) & 0x08))
    ;

  for (int idx = 0; idx < 256; idx++)
    outword(0x1F0, buffer[8 + idx * 2] | (buffer[8 + idx * 2 + 1] << 8));
  return 1;
}

void print_sector(uint32_t sector) {
  uint8_t *buff = (uint8_t *)kmalloc(SECTOR_SIZE);

  ata_read_sector(sector, buff);

  for (int i = 0; i < SECTOR_SIZE / 2; i++) {
    if (buff[i] < 0x10)
      printf("0%x", buff[i]);
    else
      printf("%x", buff[i]);
    if ((i % 2) != 0)
      putchar(' ');
  }
  println("");
  kfree((uint32_t)buff);
}
