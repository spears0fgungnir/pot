#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

typedef struct Process {
    int PID;
    char name[32];
} process_t;

void create_process(process_t* PROC, const int PID, const char* name){
    PROC->PID = PID;
    strcpy(PROC->name, name);
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

int read_file(const char* file_path, char* buf, size_t buf_size){
    FILE* fptr = fopen(file_path, "r");

    if (fptr == NULL) {
        fprintf(stderr, "Failed to read file\n");
        return 1;
    }

    while (fgets(buf, buf_size, fptr) != NULL) {}

    fclose(fptr);
    return 0;
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

int fetchProcs(process_t** proc_list) {
    int i = 0;
    struct dirent* proc;
    DIR* procs = opendir("/proc/");

    if (procs == NULL) {
        fprintf(stderr, "You're a moron and that's why it failed\n");
        return 1;
    }

    while ((proc = readdir(procs)) != NULL) {
        if (isnum(proc->d_name)) {
            proc_list[i] = malloc(sizeof(process_t));
            char* PID = proc->d_name;
            char fp[12 + strlen(PID)];
            char proc_name[32] = {0};

            sprintf(fp, "/proc/%s/comm", PID);
            read_file(fp, proc_name, 32);

            int tmp = strlen(proc_name);
            if (tmp > 0 && proc_name[tmp - 1] == '\n')
                proc_name[tmp - 1] = '\0';

            int intPID = atoi(PID);
            create_process(proc_list[i], intPID, proc_name);

            if (proc_list[i] == NULL){
                fprintf(stderr, "Go buy more RAM lol");
                return 1;
            }

            i++;
        }
    }

    closedir(procs);
    return 0;
}

int main(int argc, char* argv[]) {
    size_t proc_count = countProcs();
    process_t** proc_list = malloc(sizeof(process_t*) * proc_count);

    if (fetchProcs(proc_list) == 0){
        for (int i = 0; i < proc_count; i++){
            printf("[%d] PID: %d\t\t%s\n", i+1, proc_list[i]->PID, proc_list[i]->name);
        }
    } else {
        fprintf(stderr, "IDK What happened");
        return 1;
    }
    
    for (size_t i = 0; i < proc_count; i++)
        free(proc_list[i]);

    free(proc_list);
    return 0;
}
























