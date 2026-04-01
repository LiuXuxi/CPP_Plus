#include<stdio.h>
#include<errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdlib.h>

volatile __sig_atomic_t pid;

void sigchld_handler(int s){
    int olderrno=errno;
    pid=waitpid(-1,NULL,0);
    errno=olderrno;
}

void sigint_handler(int s){}

int main(){
    sigset_t mask,prev;

    signal(SIGCHLD,sigchld_handler);
    signal(SIGINT,sigint_handler);
    sigemptyset(&mask);
    sigaddset(&mask,SIGCLD);
    while(1){
        sigprocmask(SIG_BLOCK,&mask,&prev);
        if(fork()==0){
            //while(1);
            exit(0);
        }
        pid=0;
        sigprocmask(SIG_SETMASK,&prev,NULL);
        while(!pid);
        printf(".");
    }

    return 0;
}