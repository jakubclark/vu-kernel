# VU-Kernel

## About

`VU-Kernel` is a small kernel written in C.

The current target architecture is `i386`(32 bit version of the x86 ISA).

* The kernel has a basic ATA driver, and a FAT12 filesystem implementation is being implemented.
    * A VFS should be implemented on top of the FAT12 filesystem


## Prerequisites

A number of programs must be installed before `VU-Kernel` can be run:

- make - 4.1
    - Used to easily compile and run the kernel
- gcc - 7.3.0
    - Used to compile the `.c` files
- binutils
    - Includes `gas` which compiles `.asm` files
    - Includes `ld` which linkes `.o` files
- qemu - 2.11.1
    - Used to run the kernel itself

Take a look at [provision.sh](./resources/provision.sh)

## Running `VU-Kernel`

Running `VU-Kernel` is as simple as typing:

```
make qemu
```

This will create a bootable `.iso` file, used by `qemu`.

## `Make` targets

- kernel
    - Builds the `VU-Kernel` binary, located at `vu-kernel/kernel`
- qemu
    - Builds the `VU-Kernel` binary and runs it in qemu
- iso
    - Builds a bootable iso image, with `VU-kernel` as the boot image

## TODO

- [x] [Bootstrapping](https://jakubclark.github.io/jekyll/update/2019/04/18/bootstrapping.html)
- [x] [Printing to the screen](https://jakubclark.github.io/jekyll/update/2019/04/18/printing-to-the-screen.html)
- [x] Reading from the keyboard
- [x] Setting up the Global Descriptor Table
- [x] Print Multiboot Information on startup
- [x] Interrupt Handling
- [x] Physical Memory
- [x] Virtual Memory
- [x] ATA Driver
- [ ] FAT12
- [ ] VFS
