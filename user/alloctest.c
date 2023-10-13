//
// Created by asgrim on 13.10.23.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
#define FPP NOFILE - 3

int main(int argc, char** argv) {
    int open_files[FPP];
    int total_files = 0;
    printf("filetest: start\n");

    fork:
    if (total_files > NFILE) {
        printf("filetest: success\n");
        return 0;
        for (int i = 0; i < FPP; i++) {
            close(open_files[i]);
        }
    }
    int pid = fork();

    if (pid == 0) {
        for (int i = 0; i < FPP; i++) close(open_files[i]);
        int i;
        for (i = 0; i < FPP; i++) {
            open_files[i] = open("README", 0);
            total_files++;
            if (open_files[i] < 0) {
                printf("filetest: failed on file %d\n", total_files);
                return 1;
            }
        }
        goto fork;
    } else {
        wait(0);
        for (int i = 0; i < FPP; i++) {
            close(open_files[i]);
        }
    }

    return 0;
}