ENTRY(start)
SECTIONS
{
  . = 0x100000;
  .boot :
    {
        /* ensure that the multiboot header is at the beginning */
        *(.multiboot_header)
    }
  .text :
  {
    KERNEL_TEXT = .;
    code = .; _code = .; __code = .;
    *(.text, .text.* .rodata, .rodata.*)
    . = ALIGN(4096);
    KERNEL_TEXT_END = .;
  }
  .data :
  {
    data = .; _data = .; __data = .;
    *(.data, .data.*)
    . = ALIGN(4096);
  }
  .bss  :
  {
    bss = .; _bss = .; __bss = .;
    *(.bss,  .bss.* )
    . = ALIGN(4096);
  }

  kernel_top = .;
}
