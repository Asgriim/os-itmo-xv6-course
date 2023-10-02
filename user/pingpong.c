//
// Created by asgrim on 20.09.23.
//
#include "ulib.h"

void printErr(const char *char_ptr) {
    fprintf(2,char_ptr);
    exit(1);
}

int main() {
    char buf[256];
    int p[2];
    const char *PING_MES = "ping";
    const char *PONG_MES = "pong";
    if(pipe(p) == -1) {
        printErr("error: cannot create pipe");
    }

    if (write(p[1], PING_MES, strlen(PING_MES)) != strlen(PING_MES)) {
        printErr("error while writing PING to pipe");
    }

    int st = fork();
    if (st == -1) {
        printErr("error: cannot fork process");
    }

    if (st == 0) {
        if (read(p[0],buf, sizeof(buf)) == -1) {
            printErr("error while reading from pipe");
        }
        printf("%d: got %s \n",getpid(),buf);

        if (write(p[1], PONG_MES, strlen(PONG_MES)) != strlen(PONG_MES)) {
            printErr("error while writing PONG to pipe");
        }

        exit(0);
    } else {
        wait(0);

        if (read(p[0],buf, sizeof(buf)) == -1) {
            printErr("error while reading from pipe");
        }

        printf("%d: got %s \n",getpid(),buf);

        exit(0);
    }

}
