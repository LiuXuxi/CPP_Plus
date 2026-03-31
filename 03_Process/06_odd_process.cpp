#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include<stdlib.h>
#include <sys/wait.h>

int main(){
    //得到子进程1
    pid_t pid=fork();
    if(pid==0){
        printf("this is first child process\n");
        while(1);
        exit(0);
    }else if(pid>0){
        //父进程创建第二个子进程
        pid_t pid2=fork();
        if(pid2==0){
            printf("this is second child process\n");
            while(1);
            exit(0);
        }
        else if(pid2>0){
            //此时有三个进程
            printf("this is parent process\n");
            //等待回收一个进程资源
            wait(NULL);
            //此时有两个进程
            printf("wait one process\n");
            //等待回收另一个进程资源
            wait(NULL);
            //此时只剩父进程
            printf("wait success\n");
            return 0;
        }else{
            perror("fork failed\n");
            return -1;
        }
    }else{
        perror("fork failed\n");
        return -1;
    }
    return 0;
}