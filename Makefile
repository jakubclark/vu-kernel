ISO_FILE=kernel.iso

.PHONY: all
.PHONY: kernel
.PHONY: qemu
.PHONY: iso
.PHONY: clean

ifneq (, $(shell which grub2-mkrescue 2> /dev/null))
  GRUB_MKRESCUE = grub2-mkrescue
else ifneq (, $(shell which grub-mkrescue 2> /dev/null))
  GRUB_MKRESCUE = grub-mkrescue
else
    $(error "Cannot find grub-mkrescue or grub2-mkrescue")
endif

all: kernel

kernel:
	make -C vu-kernel all

iso: $(ISO_FILE)

$(ISO_FILE): kernel
	mkdir -p iso/boot/grub
	cp grub.cfg iso/boot/grub/
	cp vu-kernel/kernel iso/boot/
	touch kernel.iso
	$(GRUB_MKRESCUE) -o $(ISO_FILE) iso

qemu: kernel
	qemu-system-i386 -kernel vu-kernel/kernel -monitor stdio -m 32M

clean:
	make -C vu-kernel clean
	rm -rf iso
	rm -rf $(ISO_FILE)
