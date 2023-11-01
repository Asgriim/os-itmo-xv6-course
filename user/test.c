//
// Created by asgrim on 28.09.23.
//
#include "ulib.h"
// allocate all mem, free it, and allocate again
void
mem(char *s)
{
    void *m1, *m2;
    int pid;

    if((pid = fork()) == 0){
        m1 = 0;
        printf("pk \n");
        while((m2 = malloc(10001)) != 0){
//            printf("pk 1\n");
            *(char**)m2 = m1;
            m1 = m2;
        }
        printf("pk \n");
        while(m1){
            m2 = *(char**)m1;
            free(m1);
            m1 = m2;
        }
        printf("pk \n");
        m1 = malloc(1024*20);
        if(m1 == 0){
            printf("couldn't allocate mem?!!\n", s);
            exit(1);
        }
        free(m1);
        exit(0);
    } else {
        int xstatus;
        wait(&xstatus);
        if(xstatus == -1){
            // probably page fault, so might be lazy lab,
            // so OK.
            exit(0);
        }
        exit(xstatus);
    }
}
int main() {
    mem("mem");
    printf("ok");
    exit(0);
}
