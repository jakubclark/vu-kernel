#include "scrn.h"
#include "keyboard.h"
#include "idt.h"
#include "gdt.h"

void init(){
    vga_init();
    gdt_init();
    idt_init();
    kb_init();
}
