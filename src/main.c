/* main.c - Entry point for the firmware */
#include "firmware.h"
#include "supervisor.h"
#include "user_apps.h"

int main(void) {
    // Initialize the firmware in machine mode
    firmware_init();
    int n=500;
    while (n--);
    // Switch to supervisor mode
    switch_to_supervisor();
    
    // We should never reach here as control is transferred
    while(1) { /* Should never reach here */ }
    
    return 0;
}
