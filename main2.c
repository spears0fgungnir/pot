#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

typedef struct Process {
    int PID;
    char name[64];
} process_t;

struct Process_list {
    process_t** proc_list;
    size_t count;
    size_t capacity;
};

process_t* create_process(const int PID, char* name) {
    process_t* PROC = malloc(sizeof(process_t));
    if (PROC == NULL) {
        fprintf(stderr, "Go buy more RAM lol");
        return NULL;
    }
    PROC->PID = PID;
    strcpy(PROC->name, name);
    free(name);
    return PROC;
}

void init_process(struct Process_list* processes) {
    processes->count = 0;
    processes->capacity = sizeof(process_t*) * 200;
    processes->proc_list = (process_t**) malloc(processes->capacity);
}

int add_process(struct Process_list* processes, process_t* PROC) {
    if (sizeof(PROC) * (processes->count + 1) >= processes->capacity) {
        processes->capacity *= 2;
        processes->proc_list = (process_t**) realloc(processes->proc_list, processes->capacity);
        if (processes->proc_list == NULL)
            return 1;
    }
    processes->proc_list[processes->count] = PROC;
    processes->count++;
    return 0;
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

int file_len(const char* file_path){
    FILE* fptr = fopen(file_path, "rb");

    if (fptr == NULL) {
        fprintf(stderr, "Failed to read file");
        return 1;
    }

    fseek(fptr, 0, SEEK_END);
    int len = (int) ftell(fptr) + 1;
    fclose(fptr);
    return len;
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
    fprintf(stderr, "fatal: Failed to read process name");
    if (fptr != NULL)
        fclose(fptr);
    return NULL;
}

size_t countProcs(void) {
    size_t proc_count = 0;
    struct dirent* proc;
    DIR* procs = opendir("/proc/");

    if (procs == NULL) {
        fprintf(stderr, "ERROR\n"); // I'll come back to this later
        return 1;
    }

    while ((proc = readdir(procs)) != NULL)
        if (isnum(proc->d_name))
            proc_count++;

    closedir(procs);
    return proc_count;
}

int fetchProcs(struct Process_list* processes) {
    int i = 0;
    struct dirent* proc;
    DIR* procs = opendir("/proc/");

    if (procs == NULL) {
        fprintf(stderr, "You're a moron and that's why it failed\n");
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
            // printf("%s|%s|\n", fp, proc_name);

            int intPID = atoi(PID);
            process_t* new_proc = create_process(intPID, proc_name);
            if (add_process(processes, new_proc) != 0) {
                fprintf(stderr, "fatal: Cannot allocate more processes");
                return 1;
            }
            i++;
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

    struct Process_list processes;
    init_process(&processes);

    if (fetchProcs(&processes) == 0){
        for (int i = 0; i < processes.count; i++){
            printf("[%d] PID: %d\t\t%s\n", i+1, processes.proc_list[i]->PID, processes.proc_list[i]->name);
        }
    } else {
        fprintf(stderr, "IDK What happened");
        return 1;
    }
    
    for (size_t i = 0; i < processes.count; i++)
        free(processes.proc_list[i]);

    free(processes.proc_list);
    return 0;
}


