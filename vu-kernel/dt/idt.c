#include "idt.h"
#include "basicio.h"
#include "types.h"
#include "keyboard.h"

struct idt_entry IDT[IDT_SIZE];

void idt_init(void) {
  struct idt_ptr idt_ptr;
  uint32_t handler_addr = (uint32_t)keyboard_handler;

  IDT[0x21].offset_lowerbits = handler_addr & 0xffff;
  IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
  IDT[0x21].zero = 0;
  IDT[0x21].type_attr = INTERRUPT_GATE;
  IDT[0x21].offset_higherbits = (handler_addr & 0xffff0000) >> 16;

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

  idt_ptr.limit = sizeof(IDT);
  idt_ptr.base = (unsigned long)IDT;

  load_idt(&idt_ptr);
}
