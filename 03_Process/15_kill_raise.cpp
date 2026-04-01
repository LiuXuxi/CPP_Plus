#include<stdio.h>
#include <signal.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void handler(int sign){
    if(sign==SIGUSR1){
        //先等待回收子进程的资源
        wait(NULL);
        //再通知自己退出进程
        raise(SIGKILL);
    }
}

int main(){
    //SIGUSR1是用户使用的信号
    if(signal(SIGUSR1,handler)==SIG_ERR){
        perror("SIGUSR1 failed");
        return -1;
    }

    //开始父子进程间的通信
    pid_t pid=fork();
    if(pid==-1){
        perror("fork failed");
        return -1;
    }
    else if(pid==0){
        sleep(5);
        //发送信号给父进程
        kill(getppid(),SIGUSR1);
        //发送后子进程退出
        exit(0);
    }

    while(1){
        sleep(2);
        printf("parent process is running...\n");
    }

    return 0;
}