# VU-Kernel

## About

`VU-Kernel` is a small kernel written in C.

## Prerequisites

A number of programs must be installed before `VU-Kernel` can be run:

- make - 4.1
- gcc - 7.3.0
- binutils
- mtools - 4.0.18
- xorriso - 1.4.8
- grub-mkrescue (2.02) or grub2-mkrescue
- qemu - 2.11.1

Take a look at [provision.sh](./resources/provision.sh)

## Running `VU-Kernel`

Running `VU-Kernel` is as simple as typing:

```
make qemu
```

This will create a bootable `.iso` file, used by `qemu`.

## TODO

- [x] [Bootstrapping](https://jakubclark.github.io/jekyll/update/2019/04/18/bootstrapping.html)
- [x] Hello World!
- [x] [Printing to the screen](https://jakubclark.github.io/jekyll/update/2019/04/18/printing-to-the-screen.html)
- [ ] Reading from the keyboard
- [ ] Setting up the Global Descriptor Table
- [ ] Print Multiboot Information on startup
- [ ] Interrupt Handling
- [ ] Physical Memory
- [ ] Virtual Memory
