#include "io/basicio.h"
#include "io/scrn.h"
#include "drivers/floppy/floppy.h"
#include "memory/dma.h"

// static const int floppy_base = 0x03f0;
// static const int floppy_irq = 6;
static volatile int floppy_motor_ticks = 0;
static volatile int floppy_motor_state = 0;

uint8_t fdc_track = 0xff;

uint32_t floppy_irq_done = 0;

uint8_t dchange = 0;
uint8_t done = 0;
int tmout = 0;
uint8_t statsz = 0;
uint8_t status[7] = {0};
uint8_t sr0;
unsigned long tbaddr = 0x80000L;

static const char *drive_types[8] = {
    "none",         "360kB 5.25\"", "1.2MB 5.25\"", "720kB 3.5\"",

    "1.44MB 3.5\"", "2.88MB 3.5\"", "unknown type", "unknown type"};

void floppy_init() {
  outbyte(0x70, 0x10);
  uint8_t drives = inbyte(0x71);
  printf(" - Floppy drive 0: %s\n", drive_types[drives >> 4]);
  printf(" - Floppy drive 1: %s\n", drive_types[drives & 0xf]);
}

void floppy_lba_to_chs(uint32_t lba, uint32_t *head, uint32_t *track,
                       uint32_t *sector) {
  *head = (lba % (FLOPPY_SECTORS_PER_TRACK * 2)) / FLOPPY_SECTORS_PER_TRACK;
  *track = lba / (FLOPPY_SECTORS_PER_TRACK * 2);
  *sector = (lba % FLOPPY_SECTORS_PER_TRACK) + 1;
}

void delay(int n) {
  for (int i = 0; i < n; i++) {
  }
}

void floppy_wait_irq() {
  println("Waiting for floppy irq");
  while (floppy_irq_done == 0)
    ;
  println("IRQ done");
  floppy_irq_done = 0;
}

void floppy_write_ccr(uint8_t val) { outbyte(FLOPPY_CTRL, val); }

void floppy_write_dor(uint8_t val) { outbyte(FLOPPY_DOR, val); }

void floppy_write_cmd(uint8_t cmd) {
  int i;
  for (i = 0; i < 500; i++)
    if (floppy_read_status() & FLOPPY_MSR_MASK_DATAREG)
      return outbyte(FLOPPY_FIFO, cmd);
}

uint8_t floppy_read_status() { return inbyte(FLOPPY_MSR); }

uint8_t floppy_read_data() {
  int i;
  for (i = 0; i < 500; i++)
    if (floppy_read_status() & FLOPPY_MSR_MASK_DATAREG)
      return inbyte(FLOPPY_FIFO);
  printf("floppy_read_data: timeout");
  return NULL;
}

uint8_t floppy_read_ctrl() { return inbyte(FLOPPY_CTRL); }

void floppy_check_int(uint32_t *st0, uint32_t *cyl) {
  floppy_write_cmd(SENSE_INTERRUPT);
  *st0 = floppy_read_data();
  *cyl = floppy_read_data();
}

int floppy_calibrate() {
  uint32_t i, st0, cyl = 0;

  floppy_motor(1);

  for (i = 0; i < 10; i++) {
    floppy_write_cmd(RECALIBRATE);
    floppy_write_cmd(0);

    while (floppy_irq_done == 0){}

    floppy_check_int(&st0, &cyl);

    if (st0 & 0xC0) {
      static const char *status[] = {0, "error", "invalid", "drive"};
      printf("floppy_calibrate: status = %s\n", status[st0 >> 6]);
      continue;
    }

    if (!cyl) {
      floppy_motor(0);
      return 0;
    }
  }

  floppy_motor(0);
  println("floppy_calibrate: 10 retries exhausted");
  return -1;
}

int floppy_reset() {
  floppy_write_dor(0x00);
  floppy_write_dor(0x0C);

  while (floppy_irq_done == 0){}

  {
    uint32_t st0, cyl;
    floppy_check_int(&st0, &cyl);
  }

  floppy_write_ccr(0x00);

  floppy_write_cmd(SPECIFY);
  floppy_write_cmd(0xdf);
  floppy_write_cmd(0x02);

  if (floppy_calibrate(0))
    return -1;
  return 0;
}

void floppy_motor(int on) {
  if (on) {
    if (!floppy_motor_state) {
      floppy_write_dor(0x1c);
      delay(500);
    }
  } else {
    if (floppy_motor_state == floppy_motor_wait) {
      println("flopp_motor: strange, fd motor-state alread waiting ...");
    }
    floppy_motor_ticks = 300;
    floppy_motor_state = floppy_motor_wait;
  }
}

void floppy_motor_kill() {
  floppy_write_dor(0x0C);
  floppy_motor_state = floppy_motor_off;
}

uint8_t waitfdc(uint8_t sensei) {
  tmout = 50000; /* set timeout to 1 second */

  while (floppy_irq_done == 0){}

  statsz = 0;

  while ((statsz < 7) && (inbyte(FLOPPY_MSR) & (1 << 4)))
    status[statsz++] = floppy_read_data();

  if (sensei) {
    floppy_write_cmd(SENSE_INTERRUPT);
    sr0 = floppy_read_data();
    fdc_track = floppy_read_data();
  }

  done = 0;

  if (tmout)
    return 1;

  if (floppy_read_ctrl() & 0x80)
    dchange = 1;

  return 0;
}

uint32_t floppy_seek(uint32_t cyli, uint32_t head) {
  uint32_t i, st0, cyl = -1;

  floppy_motor(floppy_motor_on);

  for (i = 0; i < 10; i++) {
    floppy_write_cmd(SEEK);
    floppy_write_cmd(head << 2);
    floppy_write_cmd(cyli);

    while (floppy_irq_done == 0){}

    floppy_check_int(&st0, &cyl);

    if (st0 & 0xC0) {
      static const char *status[] = {"normal", "error", "invalid", "drive"};
      printf("floppy_seek: status = %s\n", status[st0 >> 6]);
      continue;
    }

    if (cyl == cyli) {
      floppy_motor(floppy_motor_off);
      return 1;
    }
  }

  println("floppy_seek: 10 retries exhausted");
  floppy_motor(floppy_motor_off);
  return 0;
}

uint32_t floppy_seek_track(uint8_t track) {
  if (fdc_track == track)
    return 1;

  floppy_write_cmd(SEEK);
  floppy_write_cmd(0);
  floppy_write_cmd(track);

  if (!waitfdc(1))
    return 0;

  delay(15);

  if ((sr0 != 0x20) || fdc_track != track)
    return 0;
  return 1;
}

uint32_t read_block(uint32_t block, char *buff, uint64_t num_sectors) {
  uint32_t head, track, sector;
  uint32_t result = 0, track2 = 0, i = 0;

  floppy_lba_to_chs(block, &head, &track, &sector);
  floppy_lba_to_chs(block + num_sectors, &head, &track, &sector);

  if (track != track2) {
    for (i = 0; i < num_sectors; i++)
      result = fdc_rw(block + i, buff + (i * 512), 1, 1);
    return result;
  }

  return fdc_rw(block, buff, 1, num_sectors);
}

uint32_t write_block(uint32_t block, char *buff, uint64_t num_sectors) {
  return fdc_rw(block, buff, 0, num_sectors);
}

uint32_t fdc_rw(uint32_t block, char *buff, uint32_t read,
                unsigned long num_sectors) {
  uint32_t head, track, sector, copycount = 0;
  uint8_t *p_tbaddr = (uint8_t *)0x80000;
  uint8_t *p_blockbuff = (uint8_t *)buff;

  floppy_lba_to_chs(block, &head, &track, &sector);

  floppy_motor(1);

  if (!read && buff) {
    for (copycount = 0; copycount < (num_sectors * 512); copycount++) {
      *p_tbaddr = *p_blockbuff;
      p_blockbuff++;
      p_tbaddr++;
    }
  }

  for (uint32_t tries = 0; tries < 3; tries++) {
    // Disk change?

    if (floppy_seek_track(track)) {
      floppy_motor(0);
      printf("FDC: Error seeking to track\n");
      return 0;
    }

    floppy_write_ccr(0);

    if (read) {
      dma_xfer(2, tbaddr, num_sectors * 512, 0);
      floppy_write_cmd(READ_DATA);
    } else {
      dma_xfer(2, tbaddr, num_sectors * 512, 1);
      floppy_write_cmd(WRITE_DATA);
    }

    floppy_write_cmd(head << 2);
    floppy_write_cmd(track);
    floppy_write_cmd(head);
    floppy_write_cmd(sector);
    floppy_write_cmd(2);
    floppy_write_cmd(FLOPPY_SECTORS_PER_TRACK);
    floppy_write_cmd(0x1c);
    floppy_write_cmd(0xff);

    if (!waitfdc(1)) {
      println("Time out, trying operation again after reset()");
      floppy_reset();
      return fdc_rw(block, buff, read, num_sectors);
    }

    if ((status[0] & 0xC0) == 0)
      break;
  }
  return 0;
}
