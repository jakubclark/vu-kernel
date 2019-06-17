#ifndef __FLOPPY_H
#define __FLOPPY_H

#include "std/types.h"

extern void floppy_int();

#define FLOPPY_MSR_MASK_DATAREG 128
#define FLOPPY_DMA_LEN 0x4800

#define FLOPPY_SECTORS_PER_TRACK 18

enum floppy_ports {
  FLOPPY_DOR = 0x3F2,
  FLOPPY_MSR = 0x3F4,
  FLOPPY_FIFO = 0x3F5,
  FLOPPY_CTRL = 0x3F7
};

enum floppy_commands {
  READ_TRACK = 2,
  SPECIFY = 3,
  SENSE_DRIVE_STATUS = 4,
  WRITE_DATA = 5,
  READ_DATA = 6,
  RECALIBRATE = 7,
  SENSE_INTERRUPT = 8,
  WRITE_DELETED_DATA = 9,
  READ_ID = 10,
  READ_DELETED_DATA = 12,
  FORMAT_TRACK = 13,
  DUMPREG = 14,
  SEEK = 15,
  VERSION = 16,
  SCAN_EQUAL = 17,
  PERPENDICULAR_MODE = 18,
  CONFIGURE = 19,
  LOCK = 20,
  VERIFY = 22,
  SCAN_LOW_OR_EQUAL = 25,
  SCAN_HIGH_OR_EQUAL = 29
};

typedef enum {
    floppy_dir_read = 1,
    floppy_dir_write = 2
} floppy_dir;

enum { floppy_motor_off = 0, floppy_motor_on, floppy_motor_wait };

extern void floppy_init();
extern void floppy_lba_to_chs(uint32_t lba, uint32_t *head, uint32_t *track, uint32_t *sector);
extern void delay(int n);
extern void floppy_wait_irq();
extern void floppy_write_ccr(uint8_t val);
extern void floppy_write_dor(uint8_t val);
extern void floppy_write_cmd(uint8_t cmd);
extern uint8_t floppy_read_status();
extern uint8_t floppy_read_data();
extern void floppy_check_int(uint32_t *st0, uint32_t *cyl);
extern int floppy_calibrate();
extern int floppy_reset();
extern void floppy_motor();
extern void floppy_motor_kill();
extern uint32_t floppy_seek(uint32_t cyli, uint32_t head);
extern uint32_t read_block(uint32_t block, char *buff, uint64_t num_sectors);
extern uint32_t write_block(uint32_t block, char *buff, uint64_t num_sectors);
extern uint32_t fdc_rw(uint32_t block, char *blockbuff, uint32_t read, unsigned long num_sectors);

#endif
