#include "dt/idt.h"
#include "io/basicio.h"
#include "io/keyboard.h"
#include "memory/paging.h"
#include "std/types.h"
#include "drivers/ata/ata.h"

idt_entry_t IDT[IDT_SIZE];
idt_ptr_t idtptr;

void idt_init(void) {
  // Remap PIC
  outbyte(0x20, 0x11);
  outbyte(0xA0, 0x11);

  outbyte(0x21, 0x20);
  outbyte(0xA1, 0x28);

  outbyte(0x21, 0x00);
  outbyte(0xA1, 0x00);

  outbyte(0x21, 0x01);
  outbyte(0xA1, 0x01);

  outbyte(0x21, 0xff);
  outbyte(0xA1, 0xff);

  idtptr.limit = sizeof(IDT);
  idtptr.base = (unsigned long)IDT;

  // Install Keyboard Handler
  install_ir(0x21, INTERRUPT_GATE, KERNEL_CODE_SEGMENT_OFFSET,
             keyboard_handler);

  // Install Page Fault Handler
  install_ir(0XE, INTERRUPT_GATE, KERNEL_CODE_SEGMENT_OFFSET, page_fault_main);

  // Install ATA Handler
  install_ir(46, INTERRUPT_GATE, KERNEL_CODE_SEGMENT_OFFSET, ata_int);
  install_ir(47, INTERRUPT_GATE, KERNEL_CODE_SEGMENT_OFFSET, ata_int);

  load_idt();
}

void install_ir(uint32_t i, uint16_t type_attr, uint16_t selector,
                void *irq_func) {
  uint32_t ir_addr = (uint32_t)irq_func;

  IDT[i].offset_lowerbits = (uint16_t)ir_addr & 0xFFFF;
  IDT[i].offset_higherbits = (uint16_t)(ir_addr >> 16) & 0xFFFF;
  IDT[i].zero = 0;
  IDT[i].type_attr = (uint8_t)type_attr;
  IDT[i].selector = selector;
}
