#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

int isnum(const char* s) {
    if (s == NULL || *s == '\0')
        return 0;

    for (int i=0; s[i] != '\0'; i++) {
        if (s[i] < 48 || s[i] > 57)
            return 0;
    }
    return 1;
}

int getPID(int* proc_list, int* proc_count) {
    int i = *proc_count;
    struct dirent* proc;
    DIR* procs = opendir("/proc/");

    if (procs == NULL) {
        fprintf(stderr, "You're a moron and that's why it failed");
        return 1;
    }

    while ((proc = readdir(procs)) != NULL) {
        if (isnum(proc->d_name)) {
            proc_list[i] = atoi(proc->d_name);
            i++;
        }
    }

    *proc_count = i;
    closedir(procs);
    return 0;
}

int main(int argc, char* argv[]) {
    int proc_list[512];
    int proc_count = 0;

    if (getPID(proc_list, &proc_count) == 0){
        for (int i = 0; i < proc_count; i++){
            printf("[%d] PID: %d\n", i+1, proc_list[i]);
        }
    } else {
        fprintf(stderr, "IDK What happened");
        return 1;
    }

    return 0;
}
























