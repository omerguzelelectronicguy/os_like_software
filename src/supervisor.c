/* supervisor.c - Supervisor mode implementation */
#include "supervisor.h"
#include "user_apps.h"

// Assembly function declarations
extern void supervisor_trap_handler_asm(void);
extern void sret_to_user(void* user_entry);

// Task management
task_t tasks[MAX_TASKS];
int current_task = -1;
int num_tasks = 0;

// Initialize supervisor mode
void supervisor_init(void) {
    // Setup the supervisor trap handler
    asm volatile("csrw %0, %1" : : "i"(CSR_STVEC), "r"(supervisor_trap_handler_asm));
    
    // Create user tasks
    create_task(user_app1);
    create_task(user_app2);
    
    // Initialize current task
    current_task = 0;
}

// Supervisor entry point
void supervisor_entry(void) {
    // Initialize supervisor mode
    supervisor_init();
    
    // Schedule the first task
    schedule_next_task();
    
    // We should never reach here
    while(1) { /* Should never reach here */ }
}

// Create a new task
int create_task(void* entry_point) {
    if (num_tasks >= MAX_TASKS) {
        return -1;  // No more task slots
    }
    
    int id = num_tasks++;
    tasks[id].pc = entry_point;
    tasks[id].state = TASK_READY;
    tasks[id].id = id;
    
    // Allocate a stack for the task (simplified - in a real system would use dynamic allocation)
    static char stacks[MAX_TASKS][4096];
    tasks[id].sp = &stacks[id][4096 - 64]; // Leave some space for context
    
    return id;
}

// Schedule the next task
void schedule_next_task(void) {
    // Simple round-robin scheduling
    int next_task = (current_task + 1) % num_tasks;
    
    // Update task states
    if (current_task >= 0) {
        tasks[current_task].state = TASK_READY;
    }
    
    tasks[next_task].state = TASK_RUNNING;
    current_task = next_task;
    
    // Switch to the selected user task
    switch_to_user(&tasks[current_task]);
}

// Switch to user mode and start a task
void switch_to_user(task_t* task) {
    // Set SEPC to the task entry point
    asm volatile("csrw %0, %1" : : "i"(CSR_SEPC), "r"(task->pc));
    
    // Set SPP bit to user mode (0)
    unsigned long sstatus;
    asm volatile("csrr %0, %1" : "=r"(sstatus) : "i"(CSR_SSTATUS));
    sstatus &= ~(1 << 8);  // Clear SPP bit (set to user mode)
    sstatus |= (1 << 5);   // SPIE: Enable interrupts after sret
    asm volatile("csrw %0, %1" : : "i"(CSR_SSTATUS), "r"(sstatus));
    
    // Execute sret to switch to user mode
    asm volatile("sret");
}

// C handler for supervisor mode traps
void supervisor_trap_handler(void) {
    unsigned long scause;
    asm volatile("csrr %0, %1" : "=r"(scause) : "i"(CSR_SCAUSE));
    
    // Check if it's a timer interrupt (bit 31 set and code 5)
    if ((scause & 0x80000000) && ((scause & 0x7FFFFFFF) == 5)) {
        // Timer interrupt - switch tasks
        schedule_next_task();
        return;
    }
    
    // Check if it's an ECALL from user mode (code 8)
    if ((scause & 0x7FFFFFFF) == 8) {
        // ECALL from user mode
        unsigned long sepc;
        asm volatile("csrr %0, %1" : "=r"(sepc) : "i"(CSR_SEPC));
        
        // Increment SEPC to point after the ECALL instruction
        sepc += 4;
        asm volatile("csrw %0, %1" : : "i"(CSR_SEPC), "r"(sepc));
        
        // Handle system call - here we just schedule the next task
        schedule_next_task();
        return;
    }
    
    // Handle other supervisor-level traps as needed
    // ...
    
    // If we get here, an unhandled trap occurred
    while(1) { /* Error state */ }
}
