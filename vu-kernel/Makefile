CC=gcc-7
SHARED_FLAGS = -fno-builtin -O2 -nostdinc -nostdlib -ffreestanding -g -Wall -Wextra \
               -Werror -Iinclude -MMD -mno-red-zone -fno-pie -m32
CFLAGS = $(SHARED_FLAGS)
ASFLAGS = $(SHARED_FLAGS) -Wa,--divide

KERNEL_DIR = .

OBJS := $(KERNEL_DIR)/kernel.o \
	\
	$(KERNEL_DIR)/chell/chell.o \
	$(KERNEL_DIR)/chell/commands/multiboot_cmd.o \
	\
	$(KERNEL_DIR)/dt/gdt.o \
	$(KERNEL_DIR)/dt/idt.o \
	\
	$(KERNEL_DIR)/io/basicio.o \
	$(KERNEL_DIR)/io/keyboard.o \
	$(KERNEL_DIR)/io/scrn.o \
	\
	$(KERNEL_DIR)/memory/memutil.o \
	\
	$(KERNEL_DIR)/std/string.o

DFILES = $(patsubst %.o,%.d,$(OBJS))

all: kernel

kernel: $(OBJS) Makefile kernel.ld
	nasm -f elf32 -o boot.o boot.asm
	ld -m elf_i386 -T kernel.ld -z muldefs -o kernel $(OBJS) boot.o

clean:
	find -name "*~" -delete
	rm -rf $(OBJS) $(DFILES) kernel boot.o

$(OBJS): Makefile
-include $(DFILES)
