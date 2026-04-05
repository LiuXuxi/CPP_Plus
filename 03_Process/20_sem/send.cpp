#include<stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<string.h>
#include<unistd.h>
#include"sem.h"
#define PAGESIZE 4096

int main(){
    //得到共享内存的key值
    int key=ftok("/",'k');
    if(key==-1){
        perror("ftok failed");
        return -1;
    }
    printf("key is %x\n",key);

    //创建或打开共享内存
    int shmid=shmget(key,PAGESIZE,IPC_CREAT|0664);
    if(shmid==-1){
        perror("shmget failed");
        return -1;
    }
    printf("shmid is %d\n",shmid);


    //映射共享内存
    //0表示读写操作
    //SHM_RDONLY表示只读操作
    char *addr=(char*)shmat(shmid,NULL,0);
    printf("addr is %p\n",addr);

    //创建信号量集
    int semid=create_sem(2);
    if(semid==-1){
        perror("create_sem error");
        return -1;
    }
    printf("semid is %d\n",semid);


    //向共享内存写东西
    while(1){
        //从信号量0中申请资源
        P(semid,0);

        printf("please input>>>");
        //从输入缓冲区读取数据到addr，也就是向共享内存写入数据
        fgets(addr,PAGESIZE,stdin);
        //已结束符结尾
        addr[strlen(addr)-1]=0;

        //释放资源到信号量1中
        V(semid,1);

        if(strcmp(addr,"quit")==0)break;
    }

    printf("input over...\n");

    //关闭共享内存的映射
    if(shmdt(addr)==-1){
        perror("shmdt failed");
        return -1;
    }

    //删除信号量集
    delete_sem(semid);

    return 0;
}