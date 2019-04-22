#include "scrn.h"
#include "keyboard.h"
#include "idt.h"

void init(){
    vga_init();
    idt_init();
    kb_init();
}