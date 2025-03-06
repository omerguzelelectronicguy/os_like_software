
SECTIONS
{
  .text : {
    *(.text.init)
    *(.text)
  } > RAM

  .rodata : {
    *(.rodata)
    *(.rodata.*)
  } > RAM

  .data : {
    *(.data)
    *(.data.*)
  } > RAM

  .bss : {
    *(.bss)
    *(.bss.*)
    *(COMMON)
  } > RAM

  /* Stack after bss */
  _end = .;
  
  /* Stack size and location */
  _stack_size = 4K;
  _stack_start = ORIGIN(RAM) + LENGTH(RAM) - 16;
  _stack_end = _stack_start - _stack_size;
}
