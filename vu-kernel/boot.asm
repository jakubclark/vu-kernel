bits 32
section .multiboot_header
        align 4
        dd 0x1BADB002
        dd 0x00
        dd - (0x1BADB002 + 0x00)

section .text

global start
global keyboard_handler
global load_idt
global load_gdt

extern kernel_main
extern keyboard_handler_main
extern gdtptr
extern idtptr

global load_page_directory
load_page_directory:
   mov eax, [esp+4]
   mov cr3, eax
   ret


global enable_paging
enable_paging:
   mov eax, cr0
   or eax, 0x80000000
   mov cr0, eax
   ret

load_gdt:
    ; load the new GDT pointer
    cli
    lgdt [gdtptr]
    jmp 0x08:full_load_gdt

full_load_gdt:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

load_idt:
	lidt [idtptr]
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
resb 8192 ; 8KB for stack
stack_space:
