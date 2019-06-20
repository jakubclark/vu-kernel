#include "device.h"
#include "drivers/ata/ata.h"
#include "fs/fat.h"
#include "fs/vfs.h"
#include "io/basicio.h"
#include "io/scrn.h"
#include "memory/physmem.h"
#include "std/types.h"

static device_t ata_dev_info;
uint8_t ata_irq_done = 0;

void write_sector(uint32_t addr) {
  outbyte(0x1F1, 0x00);
  outbyte(0x1F2, 0x01);
  outbyte(0x1F3, (uint8_t)addr);
  outbyte(0x1F4, (uint8_t)(addr >> 8));
  outbyte(0x1F5, (uint8_t)(addr >> 16));
  outbyte(0x1F6, 0xE0 | (drive << 4) | ((addr >> 24) & 0x0F));
  outbyte(0x1F7, 0x30);
}

void read_sector(uint32_t addr) {
  outbyte(0x1F1, 0x00);
  outbyte(0x1F2, 0x01);
  outbyte(0x1F3, (uint8_t)addr);
  outbyte(0x1F4, (uint8_t)(addr >> 8));
  outbyte(0x1F5, (uint8_t)(addr >> 16));
  outbyte(0x1F6, 0xE0 | (drive << 4) | ((addr >> 24) & 0x0F));
  outbyte(0x1F7, 0x20);
}

void init_ata() {
  // Show the first sector
  uint8_t *buff = (uint8_t *)kmalloc(512);

  ata_read_sector(0, buff);

  for (int i = 0; i < 100; i += 2) {
    if (buff[i] < 0x10)
      printf("0%x", buff[i]);
    else
      printf("%x", buff[i]);

    if (buff[i + 1] < 0x10)
      printf("0%x ", buff[i + 1]);
    else
      printf("%x ", buff[i + 1]);

    if (((i + 2) % 16) == 0 && i != 0)
      println("");
  }
  println("");

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

void ata_read_sector(uint32_t addr, uint8_t *buffer) {
  uint32_t tmpword;
  read_sector(addr);

  while (!(inbyte(0x1F7) & 0x08))
    ;

  for (int idx = 0; idx < 256; idx += 4) {
    tmpword = inword(0x1F0);
    buffer[idx] = (uint8_t)tmpword;
    buffer[idx + 1] = (uint8_t)(tmpword >> 8);
    buffer[idx + 2] = (uint8_t)(tmpword >> 16);
    buffer[idx + 3] = (uint8_t)(tmpword >> 24);
  }
}

uint32_t ata_write_sector(uint32_t addr, uint8_t *buffer) {
  uint8_t tmpword;
  uint32_t idx;

  write_sector(addr);

  while (!(inbyte(0x1F7) & 0x08))
    ;

  for (idx = 0; idx < 256; idx++) {
    tmpword = buffer[8 + idx * 2] | (buffer[8 + idx * 2 + 1] << 8);
    outword(0x1F0, tmpword);
  }
  return 1;
}
