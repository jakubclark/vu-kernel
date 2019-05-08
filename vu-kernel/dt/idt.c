#include "dt/idt.h"
#include "io/basicio.h"
#include "io/keyboard.h"
#include "memory/paging.h"
#include "std/types.h"

idt_entry_t IDT[IDT_SIZE];
idt_ptr_t idtptr;

void idt_init(void) {
  // TODO: use `register_interrupt_handler`
  uint32_t keyboard_handler_addr = (uint32_t)keyboard_handler;

  IDT[0x21].offset_lowerbits = keyboard_handler_addr & 0xffff;
  IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
  IDT[0x21].zero = 0;
  IDT[0x21].type_attr = INTERRUPT_GATE;
  IDT[0x21].offset_higherbits = (keyboard_handler_addr & 0xffff0000) >> 16;

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

  uint32_t page_fault_handler_addr = (uint32_t)page_fault_main;
  IDT[14].offset_lowerbits = page_fault_handler_addr & 0xffff;
  IDT[14].selector = KERNEL_CODE_SEGMENT_OFFSET;
  IDT[14].zero = 0;
  IDT[14].type_attr = INTERRUPT_GATE;
  IDT[14].offset_higherbits = (page_fault_handler_addr & 0xffff0000) >> 16;

  idtptr.limit = sizeof(IDT);
  idtptr.base = (unsigned long)IDT;

  load_idt();
}
