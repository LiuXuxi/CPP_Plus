#include<stdio.h>
#include <signal.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void handler(int sign){
    //阻塞回收，只回收一个
    wait(NULL);

    // //非阻塞回收，全都收完
    // while(waitpid(-1,NULL,WNOHANG));
}

int main(){
    //绑定信号捕获机制
    if(signal(SIGCLD,handler)==SIG_ERR){
        perror("SIG_ING fail");
        return -1;
    }

    //创建子进程，子进程一创建就终止等待回收
    for(int i=0;i<10;i++){
        if(fork()==0){
            exit(0);
        }
    }
    while(1);
    return 0;
}