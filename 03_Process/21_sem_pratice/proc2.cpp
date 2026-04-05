#include"sem.h"
#include<stdio.h>
#include <unistd.h>

int main(){
    //1.创建一个信号量集，包含三个信号量类型
    int semid=create_sem(3);

    while(1){
        //2.申请一个1号资源
        P(semid,1);

        //3.进行任务调度
        sleep(1);
        printf("B");
        //刷新缓冲区
        fflush(stdout);

        //4.释放一个2号资源
        V(semid,2);
    }

    return 0;
}