/* user_apps.h - User mode applications */
#ifndef USER_APPS_H
#define USER_APPS_H

// Function prototypes for user applications
void user_app1(void);
void user_app2(void);

// System call handler
void syscall(int call_number, void* arg);

// System call numbers
#define SYS_YIELD      1
#define SYS_PRINT      2
#define SYS_EXIT       3

#endif /* USER_APPS_H */

