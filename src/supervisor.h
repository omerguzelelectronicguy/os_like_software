/* supervisor.h - Supervisor mode declarations */
#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include "firmware.h"

// Task control block structure
typedef struct {
    void* sp;               // Saved stack pointer
    void* pc;               // Program counter (entry point)
    unsigned int state;     // Task state
    unsigned int id;        // Task ID
} task_t;

// Task states
#define TASK_READY      0
#define TASK_RUNNING    1
#define TASK_BLOCKED    2

// Maximum number of tasks
#define MAX_TASKS 4

// Function prototypes
void supervisor_init(void);
void supervisor_entry(void);
void switch_to_user(task_t* task);
void supervisor_trap_handler(void);
void schedule_next_task(void);
int create_task(void* entry_point);

#endif /* SUPERVISOR_H */
