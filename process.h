#pragma once 
#include <stddef.h>

typedef enum Signal {
    SIGTERM,
    SIGKILL,
    SIGSTOP,
    SIGCONT,
    SIGHUP
} SIGNAL_T;

typedef struct Process {
    int PID;
    char name[64];
} process_t;

process_t* create_process(char* PID); // Automatically retrieves its name

