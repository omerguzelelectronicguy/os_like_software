/* firmware.c - Firmware implementation */
#include "firmware.h"

// Assembly function declarations
extern void mret_to_supervisor(void* supervisor_entry);
extern void machine_trap_handler_asm(void);

// Firmware initialization
void firmware_init(void) {
    // Initialize PMP to allow access from all privilege modes
    // Set PMP configuration for region 0 (NAPOT: Naturally aligned power of 2)
    asm volatile("csrw %0, %1" : : "i"(CSR_PMPCFG0), "r"(0x1F));
    
    // Set PMP address for region 0 (all memory)
    unsigned long all_mem = -1;
    all_mem >>= 1;  // Address is shifted right by 2
    asm volatile("csrw %0, %1" : : "i"(CSR_PMPADDR0), "r"(all_mem));
    
    // Enable FPU
    unsigned long mstatus;
    asm volatile("csrr %0, %1" : "=r"(mstatus) : "i"(CSR_MSTATUS));
    mstatus |= (1 << 13);  // Set FS bits to enable FPU
    asm volatile("csrw %0, %1" : : "i"(CSR_MSTATUS), "r"(mstatus));
    
    // Setup trap handler vector
    asm volatile("csrw %0, %1" : : "i"(CSR_MTVEC), "r"(machine_trap_handler_asm));
    
    // Delegate exceptions and interrupts to supervisor mode
    // MEDELEG: Delegate exceptions
    asm volatile("csrw %0, %1" : : "i"(CSR_MEDELEG), "r"(0x100)); // ECALL from U-mode
    
    // Setup the timer for a future interrupt
    setup_timer_interrupt(1000000); // 1 second with a 1MHz clock
}

// Set up the timer for a future interrupt
void setup_timer_interrupt(unsigned long interval) {
    volatile unsigned long* mtime = (volatile unsigned long*)MTIME_ADDR;
    volatile unsigned long* mtimecmp = (volatile unsigned long*)MTIMECMP_ADDR;
    
    // Set timer compare value to current time + interval
    *mtimecmp = *mtime + interval;
    
    // Enable timer interrupts
    unsigned long mie;
    asm volatile("csrr %0, mie" : "=r"(mie));
    mie |= (1 << 7); // MTIE: Machine Timer Interrupt Enable
    asm volatile("csrw mie, %0" : : "r"(mie));
}

// C handler for machine mode traps
void machine_trap_handler(void) {
    unsigned long mcause;
    asm volatile("csrr %0, %1" : "=r"(mcause) : "i"(CSR_MCAUSE));
    
    // Check if it's a timer interrupt (bit 31 set and code 7)
    if ((mcause & 0x80000000) && ((mcause & 0x7FFFFFFF) == 7)) {
        // Timer interrupt: reschedule timer and continue
        setup_timer_interrupt(1000000);  // Reset timer for next interrupt
        
        // Return to where execution was interrupted
        return;
    }
    
    // Handle other machine-level traps as needed
    // ...

    // If we get here, an unhandled trap occurred
    while(1) { /* Error state */ }
}

// Function to switch to supervisor mode
void switch_to_supervisor(void) {
    extern void supervisor_entry(void);
    
    // Set MEPC to the supervisor entry point
    asm volatile("csrw %0, %1" : : "i"(CSR_MEPC), "r"(supervisor_entry));
    
    // Set MPP (Previous Privilege) bits to supervisor mode (01)
    unsigned long mstatus;
    asm volatile("csrr %0, %1" : "=r"(mstatus) : "i"(CSR_MSTATUS));
    mstatus &= ~MSTATUS_MPP_MASK;     // Clear MPP bits
    mstatus |= MSTATUS_MPP_S;         // Set MPP to supervisor mode
    mstatus |= MSTATUS_MPIE;          // Enable interrupts after mret
    asm volatile("csrw %0, %1" : : "i"(CSR_MSTATUS), "r"(mstatus));
    
    // Execute mret to switch to supervisor mode
    asm volatile("mret");
}
