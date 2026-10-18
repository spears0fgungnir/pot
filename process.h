#pragma once 
#include <stddef.h>

typedef struct Process {
    int PID;
    char name[64];
    char state;
    size_t mem_usage;
    size_t cpu_usage;
} process_t;

process_t* create_process(char* PID); // Automatically retrieves its name

