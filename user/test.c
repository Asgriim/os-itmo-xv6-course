//
// Created by asgrim on 28.09.23.
//
#include "ulib.h"
// allocate all mem, free it, and allocate again
void
sbrkbasic(char *s)
{
    enum { TOOMUCH=1024*1024*1024};
    int i, pid, xstatus;
    char *c, *a, *b;
    printf("test pid = %d\n", getpid());
    printf("xstat ad = %p\n", &xstatus);
    // does sbrk() return the expected failure value?
    pid = fork();
    if(pid < 0){
        printf("fork failed in sbrkbasic\n");
        exit(1);
    }
    if(pid == 0){
        printf("aboba\n");
        a = sbrk(TOOMUCH);
        printf("aboba 23\n");
        if(a == (char*)0xffffffffffffffffL){
            printf("hui \n");
            // it's OK if this fails.
            exit(0);
        }

        for(b = a; b < a+TOOMUCH; b += 4096){
            *b = 99;
        }
        printf("aboba 1\n");
        // we should not get here! either sbrk(TOOMUCH)
        // should have failed, or (with lazy allocation)
        // a pagefault should have killed this process.
        exit(1);
    }

    wait(&xstatus);
    printf("aboba 2\n");
    if(xstatus == 1){
        printf("aboba 2\n");
        printf("%s: too much memory allocated!\n", s);
        exit(1);
    }

    // can one sbrk() less than a page?
    a = sbrk(0);
    printf("aboba 3\n");
    for(i = 0; i < 5000; i++){
        b = sbrk(1);
        if(b != a){
            printf("%s: sbrk test failed %d %x %x\n", s, i, a, b);
            exit(1);
        }
        *b = 1;
        a = b + 1;
    }
    pid = fork();
    if(pid < 0){
        printf("%s: sbrk test fork failed\n", s);
        exit(1);
    }
    c = sbrk(1);
    c = sbrk(1);
    if(c != a + 1){
        printf("%s: sbrk test failed post-fork\n", s);
        exit(1);
    }
    if(pid == 0)
        exit(0);
    wait(&xstatus);
    exit(xstatus);
}
int main() {
    sbrkbasic("mem");
    printf("ok");
    exit(0);
}
