#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>

int main(){
    pid_t pid=fork();
    while(1){
        if(pid==-1){
        perror("fork failed");
        return -1;
    }
        else if(pid==0){
            printf("this is child process,pid is %d,ppid is %d...\n",getpid(),getppid());
            sleep(1);
        }
        else{
            printf("this is parent process,pid is %d,ppid is %d...\n",getpid(),getppid());
            sleep(1);
        }
    }
    return 0;
}