bits 32
section .text
        align 4
        dd 0x1BADB002
        dd 0x00
        dd - (0x1BADB002 + 0x00)

global start
global keyboard_handler
global load_idt
global load_gdt

extern kernel_main
extern keyboard_handler_main

load_gdt:
	mov edx, [esp + 4]
	lgdt [edx]
	ret

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti
	ret

keyboard_handler:  
	call keyboard_handler_main
	iretd

start:
	mov esp, stack_space
	push ebx
	push eax
	call kernel_main
	cli
loop:
	hlt
	jmp loop

section .bss
resb 8192; 8KB for stack
stack_space:
