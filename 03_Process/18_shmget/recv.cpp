#include<stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<string.h>
#include <unistd.h>
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

    //向共享内存写东西
    while(1){
        sleep(2);
        //直接向共享内存读取数据
        printf("%s\n",addr);
        if(strcmp(addr,"quit")==0)break;
    }

    //关闭共享内存的映射
    if(shmdt(addr)==-1){
        perror("shmdt failed");
        return -1;
    }

    //删除共享内存
    if(shmctl(shmid,IPC_RMID,NULL)==-1){
        perror("shmctl failed");
        return -1;
    }
    return 0;
}