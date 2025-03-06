# crt0.S - Initial startup code

.section .text.init
.global _start
.align 2
_start:
    # Setup stack pointer
    la sp, _stack_start
    
    # Clear BSS section
    la t0, _bss_start
    la t1, _bss_end
    beq t0, t1, bss_done
bss_clear:
    sw zero, 0(t0)
    addi t0, t0, 4
    blt t0, t1, bss_clear
bss_done:

    # Jump to C code
    jal ra, main
    
    # If main returns, loop indefinitely
loop:
    j loop

