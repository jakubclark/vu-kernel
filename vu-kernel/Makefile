CC=gcc-7
SHARED_FLAGS = -fno-builtin -O2 -nostdinc -nostdlib -ffreestanding -g -Wall -Wextra \
               -Werror -Iinclude -MMD -mno-red-zone -fno-pie -m32
CFLAGS = $(SHARED_FLAGS)
ASFLAGS = $(SHARED_FLAGS) -Wa,--divide

OBJS := kernel.o
ALL_OBJS = $(OBJS) chell.o basicio.o gdt.o idt.o keyboard.o memutil.o scrn.o string.o
DFILES = $(patsubst %.o,%.d,$(OBJS))

all: kernel

kernel: subdir $(OBJS) Makefile kernel.ld
	nasm -f elf32 -o boot.o boot.asm
	ld -m elf_i386 -T kernel.ld -z muldefs -o kernel $(ALL_OBJS) boot.o

subdir:
	make -C chell
	make -C dt
	make -C io
	make -C memory
	make -C std
	cp chell/*.o .
	cp dt/*.o .
	cp io/*.o .
	cp memory/*.o .
	cp std/*.o .

clean:
	make -C chell clean
	make -C dt clean
	make -C io clean
	make -C memory clean
	make -C std clean
	find -name "*~" -delete
	rm -rf $(OBJS) $(DFILES) kernel boot.o

$(OBJS): Makefile
-include $(DFILES)
