/* firmware.h - Firmware declarations */
#ifndef FIRMWARE_H
#define FIRMWARE_H

// CSR addresses definitions
#define CSR_MSTATUS     0x300
#define CSR_MEPC        0x341
#define CSR_MCAUSE      0x342
#define CSR_MIP         0x344
#define CSR_MTVEC       0x305
#define CSR_PMPCFG0     0x3A0
#define CSR_PMPADDR0    0x3B0
#define CSR_MEDELEG     0x302

#define CSR_SSTATUS     0x100
#define CSR_SEPC        0x141
#define CSR_SCAUSE      0x142
#define CSR_SIP         0x144
#define CSR_STVEC       0x105

// Privilege mode bits
#define MSTATUS_MPP_MASK    0x1800  // Machine Previous Privilege mode
#define MSTATUS_MPP_M       0x1800  // Machine mode
#define MSTATUS_MPP_S       0x0800  // Supervisor mode
#define MSTATUS_MPP_U       0x0000  // User mode
#define MSTATUS_MPIE        0x0080  // Machine Previous Interrupt Enable

// PLIC and timer registers
#define MTIME_ADDR      0x200BFF8
#define MTIMECMP_ADDR   0x2004000

// Function prototypes
void firmware_init(void);
void switch_to_supervisor(void);
void machine_trap_handler(void);
void setup_timer_interrupt(unsigned long interval);

#endif /* FIRMWARE_H */
