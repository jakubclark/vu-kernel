MBOOT_PAGE_ALIGN	equ 1<<0
MBOOT_MEM_MAP		equ 1<<1
MBOOT_VIDEO_MODE	equ 1<<2
MBOOT_HEADER_MAGIC	equ 0x1BADB002
MBOOT_HEADER_FLAGS	equ MBOOT_PAGE_ALIGN | MBOOT_MEM_MAP
MBOOT_CHECKSUM		equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[bits 32]

[section .multiboot_header]
dd MBOOT_HEADER_MAGIC	; header value for GRUB
dd MBOOT_HEADER_FLAGS	; grub settings
dd MBOOT_CHECKSUM ; ensure above values are correct

[section .text]

global start
global keyboard_handler
global load_idt
global load_gdt
global enable_paging
global load_page_directory
global page_fault_main

extern kernel_main
extern keyboard_handler_main
extern gdtptr
extern idtptr
extern page_fault

extern ata_irq_done

global ata_int
ata_int:
    pushad
    inc byte [ata_irq_done]
    mov al, 0x20
    out 0xA0, al
    out 0x20, al
    popad
    iretd

page_fault_main:
    pusha

    mov ax, ds
    push eax

    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call page_fault

    pop gs
    pop fs
    pop es

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8
    sti
    iret

load_page_directory:
   mov eax, [esp+4]
   mov cr3, eax
   ret

enable_paging:
   mov eax, cr0
   or eax, 0x80000000
   mov cr0, eax
   ret

load_gdt:
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

[section .bss]
resb 8192
stack_space:
