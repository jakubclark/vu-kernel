# VU-Kernel

## About

`VU-Kernel` is a small kernel written in C.

## Prerequisites

A number of programs must be installed before `VU-Kernel` can be run:

- make
- gcc
- binutils
- mtools
- xorriso
- grub-mkrescue or grub2-mkrescue
- qemu

Take a look at [provision.sh](./resources/provision.sh)

## Running `VU-Kernel`

Running `VU-Kernel` is as simple as typing:

```
make qemu
```

This will create a bootable `.iso` file, used by `qemu`.

## TODO

- [x] Bootstrapping
- [x] Hello World!
- [x] Printing to the screen
- [ ] Reading from the keyboard
- [ ] Setting up the Global Descriptor Table
- [ ] Print Multiboot Information on startup
- [ ] Interrupt Handling
- [ ] Physical Memory
- [ ] Virtual Memory
