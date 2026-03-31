#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>

int main(){
    //出现两个进程
    fork();
    //出现四个进程
    fork();
    //出现八个进程
    fork();
    //可以发现出现八个进程，说明子进程也可以创建它的子进程
    while(1);
    return 0;
}