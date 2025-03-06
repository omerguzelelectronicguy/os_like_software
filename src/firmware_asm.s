# firmware_asm.S - Assembly functions for the firmware

.section .text
.global machine_trap_handler_asm
.align 2
machine_trap_handler_asm:
    jal     x0, machine_exception_handler
    jal     x0, machine_exception_handler # supervisor_handler_software_int
    .word   0x0
    jal     x0, machine_exception_handler # machine_handler_software_int
    .word   0x0
    jal     x0, machine_exception_handler # supervisor_handler_timer_int
    .word   0x0
    jal     x0, machine_exception_handler # machine_handler_timer_int
    .word   0x0
    jal     x0, machine_exception_handler # supervisor_handler_external_int
    .word   0x0
    jal     x0, machine_exception_handler # machine_handler_external_int
machine_exception_handler:
    # Save registers
    addi sp, sp, -16*4
    sw ra, 0*4(sp)
    sw t0, 1*4(sp)
    sw t1, 2*4(sp)
    sw t2, 3*4(sp)
    sw t3, 4*4(sp)
    sw t4, 5*4(sp)
    sw t5, 6*4(sp)
    sw t6, 7*4(sp)
    sw a0, 8*4(sp)
    sw a1, 9*4(sp)
    sw a2, 10*4(sp)
    sw a3, 11*4(sp)
    sw a4, 12*4(sp)
    sw a5, 13*4(sp)
    sw a6, 14*4(sp)
    sw a7, 15*4(sp)
    
    # Call C handler
    jal ra, machine_trap_handler
    
    # Restore registers
    lw ra, 0*4(sp)
    lw t0, 1*4(sp)
    lw t1, 2*4(sp)
    lw t2, 3*4(sp)
    lw t3, 4*4(sp)
    lw t4, 5*4(sp)
    lw t5, 6*4(sp)
    lw t6, 7*4(sp)
    lw a0, 8*4(sp)
    lw a1, 9*4(sp)
    lw a2, 10*4(sp)
    lw a3, 11*4(sp)
    lw a4, 12*4(sp)
    lw a5, 13*4(sp)
    lw a6, 14*4(sp)
    lw a7, 15*4(sp)
    addi sp, sp, 16*4
    
    # Return from trap
    mret

.global supervisor_trap_vector
.align 2
supervisor_trap_vector:
    # This will be implemented in supervisor.S
    j supervisor_trap_handler_asm
    