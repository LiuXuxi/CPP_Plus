#include <pthread.h>
#include<stdio.h>
#include<unistd.h>

pthread_t ntid;

void printids(const char* s){
    //当前进程号
    pid_t pid;
    //当前线程号
    pthread_t tid;
    pid=getpid();
    tid=pthread_self();
    printf("%s pid %lu tid %lu (0x%lu)\n",s,(unsigned long)pid,(unsigned long)tid,(unsigned long)tid);
}

void* task(void*arg){
    printids("new thread:");
    return ((void*)0);
}

int main(){
    //手动设置错误码，接受线程创建的状况
    int err;
    //创建线程，线程执行printids函数，线程号为ntid
    err=pthread_create(&ntid,NULL,task,NULL);
    //成功返回0，失败返回非0
    if(err!=0){
        printf("pthread create failed\n");
        return -1;
    }
    //主线程执行printids函数
    printids("main pthread:");
    //主线程等待分支线程结束
    sleep(1);
    return 0;

}