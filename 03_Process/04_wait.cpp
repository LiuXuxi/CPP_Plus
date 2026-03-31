#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/wait.h>

int main(){
    pid_t pid=fork();
    if(pid==-1){
        perror("fork failed");
        return -1;
    }
    else if(pid==0){
        printf("this is child process,pid is %d,ppid is %d...\n",getpid(),getppid());
        sleep(8);
        exit(0);
    }
    else{
        printf("this is parent process,pid is %d,ppid is %d...\n",getpid(),getppid());
        sleep(3);
        //wait(NULL);              //阻塞等待一个子进程退出，挂起状态
        //waitpid(-1,NULL,WNOHANG);  //非阻塞等待一个进程退出，若未回收资源，则子进程僵尸态
        waitpid(-1,NULL,0);        //阻塞等待，相当于wait(NULL);
    }
    while(1);
    return 0;
}