MEMORY
{
  sram       : ORIGIN = 0x80000000, LENGTH = 128K
  flash(rx)  : ORIGIN = 0x90000000, LENGTH = 6K
  ddr        : ORIGIN = 0xA0000000, LENGTH = 128M
}

SECTIONS
{
  .text : {
    *(.text.init)
    *(.text)
  } > sram

  .rodata : {
    *(.rodata)
    *(.rodata.*)
  } > sram

  .data : {
    *(.data)
    *(.data.*)
  } > sram

  .bss : {
    _bss_start = .;
    *(.bss)
    _bss_end = .;
    *(.bss.*)
    *(COMMON)
  } > sram

  /* Stack after bss */
  _end = .;
  
  /* Stack size and location */
  _stack_size = 4K;
  _stack_start = ORIGIN(sram) + LENGTH(sram) - 16;
  _stack_end = _stack_start - _stack_size;
}
