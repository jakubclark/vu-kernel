#include "idt.h"
#include "basicio.h"
#include "types.h"
#include "keyboard.h"

struct idt_entry IDT[IDT_SIZE];

void idt_init(void) {
  struct idt_ptr idt_ptr;

  /* populate IDT entry of keyboard's interrupt */
  uint32_t keyboard_address = (uint32_t)keyboard_handler;
  IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
  IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
  IDT[0x21].zero = 0;
  IDT[0x21].type_attr = INTERRUPT_GATE;
  IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

  /*     Ports
   *	 PIC1	PIC2
   *Command 0x20	0xA0
   *Data	 0x21	0xA1
   */

  /* ICW1 - begin initialization */
  outbyte(0x20, 0x11);
  outbyte(0xA0, 0x11);

  /* ICW2 - remap offset address of IDT */
  /*
   * In x86 protected mode, we have to remap the PICs beyond 0x20 because
   * Intel have designated the first 32 interrupts as "reserved" for cpu
   * exceptions
   */
  outbyte(0x21, 0x20);
  outbyte(0xA1, 0x28);

  /* ICW3 - setup cascading */
  outbyte(0x21, 0x00);
  outbyte(0xA1, 0x00);

  /* ICW4 - environment info */
  outbyte(0x21, 0x01);
  outbyte(0xA1, 0x01);
  /* Initialization finished */

  /* mask interrupts */
  outbyte(0x21, 0xff);
  outbyte(0xA1, 0xff);

  /* fill the IDT descriptor */
  idt_ptr.limit = sizeof(IDT);
  idt_ptr.base = (unsigned long)IDT;

  load_idt(&idt_ptr);
}
