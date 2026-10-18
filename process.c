#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

char* read_line(const char* file_path, size_t mem_size) {
    FILE* fptr = fopen(file_path, "rb");

    if (mem_size < 1)
        goto on_error;

    if (fptr == NULL)
        goto on_error;

    char* file_line = (char*) malloc(mem_size);
    if (file_line == NULL) {
        free(file_line);
        goto on_error;
    }

    if(fgets(file_line, mem_size, fptr) == NULL) {
        free(file_line);
        goto on_error;
    }

    int tmp = strlen(file_line);
    if (tmp > 0 && file_line[tmp - 1] == '\n')
        file_line[tmp - 1] = '\0';

    fclose(fptr);
    return file_line;

on_error:
    fprintf(stderr, "fatal: Failed to read process name.");
    if (fptr != NULL)
        fclose(fptr);
    return NULL;
}


char* read_name_process(char* PID) {
    char fp[sizeof("/proc//comm") + strlen(PID)];
    snprintf(fp, sizeof(fp), "/proc/%s/comm", PID);

    int tmp = strlen(fp);
    if (tmp > 0 && fp[tmp - 1] == '\n')
        fp[tmp - 1] = '\0';

    char* proc_name = read_line(fp, 64);
    return proc_name;
}

process_t* create_process(char* PID) {
    char* proc_name = read_name_process(PID);
    if (proc_name == NULL) {
        fprintf(stderr, "fatal: Unable to retrieve process name");
        return NULL;
    }

    process_t* tmp = (process_t*) malloc(sizeof(process_t));
    if (tmp == NULL) {
        fprintf(stderr, "fatal: Failed to allocate memory.");
        return NULL;
    }

    process_t* new_proc = tmp;
    new_proc->PID = atoi(PID);
    strcpy(new_proc->name, proc_name);
    free(proc_name);
    return new_proc;
}
