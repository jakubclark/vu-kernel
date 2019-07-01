KERNEL_DIR=vu-kernel
ISO_FILE=kernel.iso
BIN_FILE=$(KERNEL_DIR)/kernel

.PHONY: all kernel qemu iso clean

ifneq (, $(shell which grub2-mkrescue 2> /dev/null))
  GRUB_MKRESCUE = grub2-mkrescue
else ifneq (, $(shell which grub-mkrescue 2> /dev/null))
  GRUB_MKRESCUE = grub-mkrescue
else
    $(error "Cannot find grub-mkrescue or grub2-mkrescue")
endif

MEM := 128M

all: kernel

kernel:
	make -C vu-kernel all

iso: $(ISO_FILE)

$(ISO_FILE): kernel
	grub-file --is-x86-multiboot $(BIN_FILE)
	mkdir -p iso/boot/grub
	cp grub.cfg iso/boot/grub/
	cp $(BIN_FILE) iso/boot/
	touch kernel.iso
	$(GRUB_MKRESCUE) -o $(ISO_FILE) iso

qemu: iso
	qemu-system-i386 -hda hda.img -cdrom $(ISO_FILE) -monitor stdio -m $(MEM) -boot d

clean:
	make -C vu-kernel clean
	rm -rf iso
	rm -rf $(ISO_FILE)
