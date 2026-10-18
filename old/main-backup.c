#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include "vector.h"

typedef struct Process {
    int PID;
    char name[64];
} process_t;

/*  proc_list -> Process_list.items;
 *  count -> Process_list.count;
 *  capacity -> Process_list.capacity;
 */

process_t* create_process(const int PID, char* name) {
    process_t* PROC = malloc(sizeof(process_t));
    if (PROC == NULL) {
        fprintf(stderr, "fatal: Failed to allocate memory\n");
        return NULL;
    }
    PROC->PID = PID;
    strcpy(PROC->name, name);
    if (PROC->name == NULL) {
        fprintf(stderr, "fatal: Unable to retrieve process name\n");
        return NULL;
    }
    // free(name);
    return PROC;
}

int isnum(const char* s) {
    if (s == NULL || *s == '\0')
        return 0;

    for (int i=0; s[i] != '\0'; i++) {
        if (s[i] < 48 || s[i] > 57)
            return 0;
    }
    return 1;
}

char* read_file(const char* file_path, size_t* buf_size){
    FILE* fptr = fopen(file_path, "rb");

    if (fptr == NULL)
        goto on_error;

    if (fseek(fptr, 0, SEEK_END) != 0) 
        goto on_error;

    int fsize = (int) ftell(fptr);
    if (fsize < 0) 
        goto on_error;

    if (fseek(fptr, 0, SEEK_SET) != 0)
        goto on_error;

    char* buf = (char*) malloc(fsize + 1);
    if (buf == NULL) 
        goto on_error;
    
    size_t byte_count = fread(buf, 1, fsize, fptr);
    buf[byte_count] = '\0';
    
    if (buf_size != NULL)
        *buf_size = byte_count;

    fclose(fptr);
    return buf;

on_error:
    fprintf(stderr, "fatal: Failed to read file.\n");
    if (fptr != NULL)
        fclose(fptr);
    return NULL;
}

char* read_proc_name(const char* file_path){
    FILE* fptr = fopen(file_path, "rb");

    if (fptr == NULL)
        goto on_error;

    char* proc_name = (char*) malloc(64);
    if(fgets(proc_name, 64, fptr) == NULL)
        goto on_error;

    int tmp = strlen(proc_name);
    if (tmp > 0 && proc_name[tmp - 1] == '\n')
        proc_name[tmp - 1] = '\0';

    fclose(fptr);
    return proc_name;

on_error:
    fprintf(stderr, "fatal: Failed to read process name.");
    if (fptr != NULL)
        fclose(fptr);
    return NULL;
}

int fetch_processes(vec_t* processes) {
    struct dirent* proc;
    DIR* procs = opendir("/proc/");

    if (procs == NULL) {
        fprintf(stderr, "fatal: Failed to fetch processes");
        return 1;
    }

    while ((proc = readdir(procs)) != NULL) {
        if (isnum(proc->d_name)) {
            char* PID = proc->d_name;
            char fp[12 + strlen(PID)];

            sprintf(fp, "/proc/%s/comm", PID);
            int tmp = strlen(fp);
            if (tmp > 0 && fp[tmp - 1] == '\n')
                fp[tmp - 1] = '\0';

            char* proc_name = read_proc_name(fp);
            if (proc_name == NULL)
                return 1;

            // printf("%s|%s|\n", fp, proc_name);

            int intPID = atoi(PID);
            process_t* new_proc = create_process(intPID, proc_name);
            if (new_proc == NULL)
                return 1;

            if (add_vector_index(processes, new_proc, (size_t) intPID) != 0)
                return 1;

        }
    }

    closedir(procs);
    return 0;
}

int main(int argc, char* argv[]) {
    /*
    size_t proc_count = countProcs();
    process_t** proc_list = malloc(sizeof(process_t*) * proc_count);
    */

    vec_t processes;
    if (init_vector(&processes) != 0)
        goto terminate;

    if (fetch_processes(&processes) != 0)
        goto terminate;

    for (size_t i = 0; i < processes.max_index; i++)
        if (processes.items[i] != NULL)
            printf("PID: %d\t\t%s\n", i, ((process_t*)processes.items[i])->name);

    free_vector(&processes);
    return 0;

terminate:
    fprintf(stderr, "Terminating program.............");
    free_vector(&processes);
    return 1;
}


