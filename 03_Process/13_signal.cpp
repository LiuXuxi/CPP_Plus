#include<stdio.h>
#include <signal.h>
#include<unistd.h>

void handler(int sign){
    printf("want to exit...\n");
}

int main(){
    //信号处理方式有三种：默认、忽略、捕获

    // //忽略SIGINT，按下ctl+C时不会退出进程
    // if(signal(SIGINT,SIG_IGN)==SIG_ERR){
    //     perror("SIG_ING fail");
    //     return -1;
    // }

    // //默认SIGINT，退出进程
    // if(signal(SIGINT,SIG_IGN)==SIG_ERR){
    //     perror("SIG_ING fail");
    //     return -1;
    // }


    //捕获SIGINT，执行自定义处理函数
    if(signal(SIGINT,handler)==SIG_ERR){
        perror("SIG_ING fail");
        return -1;
    }

    while(1){
        sleep(1);
        printf("this is running...\n");
    }
    return 0;
}