#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>

int main(){
    pid_t pid=fork();
    if(pid==-1){
        perror("fork failed");
        return -1;
    }
    else if(pid==0){
        printf("this is child process,pid=%d,ppid=%d\n",getpid(),getppid());
        //子进程不退出
        while(1){
            sleep(2);
            printf("this is child process,pid=%d,ppid=%d\n",getpid(),getppid());
        }
    }
    else{
        sleep(1);
        //父进程直接退出
        return 0;
    }
    return 0;
}