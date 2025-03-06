/* user_apps.c - User mode applications */
#include "user_apps.h"

// System call implementation using ECALL instruction
void syscall(int call_number, void* arg) {
    register int a0 asm("a0") = call_number;
    register void* a1 asm("a1") = arg;
    
    asm volatile(
        "ecall"
        : "+r"(a0)
        : "r"(a1)
        : "memory"
    );
}

// User application 1 - Counter
void user_app1(void) {
    int counter = 0;
    
    while (1) {
        // Simulated work
        for (int i = 0; i < 1000; i++) {
            counter++;
        }
        
        // Yield to other tasks
        syscall(SYS_YIELD, 0);
    }
}

// User application 2 - Fibonacci calculator
void user_app2(void) {
    int a = 0, b = 1, c;
    
    while (1) {
        // Calculate next Fibonacci number
        c = a + b;
        a = b;
        b = c;
        
        // Yield to other tasks
        syscall(SYS_YIELD, 0);
    }
}/* main.c - Entry point for the firmware */
#include "firmware.h"
#include "supervisor.h"
#include "user_apps.h"

int main(void) {
    // Initialize the firmware in machine mode
    firmware_init();
    
    // Switch to supervisor mode
    switch_to_supervisor();
    
    // We should never reach here as control is transferred
    while(1) { /* Should never reach here */ }
    
    return 0;
}
