; Copyright (C) 2014  Arjun Sreedharan
; License: GPL version 2 or higher http://www.gnu.org/licenses/gpl.html

bits 32
section .text
        ;multiboot spec
        align 4
        dd 0x1BADB002              ;magic
        dd 0x00                    ;flags
        dd - (0x1BADB002 + 0x00)   ;checksum. m+f+c should be zero

global start
global keyboard_handler
global load_idt

extern Kernel_Main 		;this is defined in the c file
extern keyboard_handler_main

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti 				;turn on interrupts
	ret

keyboard_handler:                 
	call    keyboard_handler_main
	iretd

start:
	cli 				;block interrupts
	mov esp, stack_space
	call Kernel_Main
	hlt 				;halt the CPU

section .bss
resb 8192; 8KB for stack
stack_space:
