#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

int chop_by_delim(char* data, char* dest, char delim, size_t field) {
    int i = 0, c = 0, start = 0, end = 0;
    while (i < field) {
        if (data[c] != delim) {
            while (data[c] != delim)
                c++;
            start = i;
            end = c;
            i++;
        }
    }
    char buf[end - start + 1];
    for (int i = start; i <= end; i++)
        buf[i - start] = data[i];

    buf[end - start + 1] = '\0';
    strcpy(dest, buf);
    return 0;
}

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

    char fp[sizeof("/proc//stat" + strlen(PID))];
    snprintf(fp, sizeof(fp), "/proc/%s/stat", PID);

    char* stats = read_line(fp, 1024);
    // work from here 

    return new_proc;
}
