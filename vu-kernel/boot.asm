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

extern Kernel_Main
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
	push ds
	push es
	push fs
	push gs
	pushad
	call keyboard_handler_main
	popad
	pop gs
	pop fs
	pop es
	pop ds
	iretd

start:
	mov esp, stack_space
	call Kernel_Main
	cli
loop:
	hlt
	jmp loop

section .bss
resb 8192; 8KB for stack
stack_space:
