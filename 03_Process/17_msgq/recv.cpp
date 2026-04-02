#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#define MSGSZ sizeof(msgBuf)-sizeof(long)

typedef struct msgBuf{
    long mtype;
    char mtext[1024];
}msgBuf;

int main(){
    //1.获取键值，用于消息队列、共享内存和信号集的创建
    int key=ftok("/",'k');
    //16进制输出key值
    printf("key is %x\n",key);

    //2.创建消息队列，并返回队列号
    int msqid=msgget(key,IPC_CREAT|0644);
    if(msqid==-1){
        perror("msqid creat failed");
        return -1;
    }
    printf("msqid is %d\n",msqid);

    //3.接受数据
    msgBuf buf;
    while(1){
        //清空消息容器，防止错误输出
        memset(buf.mtext,0,sizeof(buf.mtext));
        //接受消息，当无指定类型消息，阻塞接受
        if(msgrcv(msqid,&buf,MSGSZ,1,0)==-1){
            perror("msgrcv failed");
            return -1;
        }
        printf("接受的消息为：%s\n",buf.mtext);
        if(strcmp(buf.mtext,"quit")==0)break;
    }
    //4.接受消息后，其余消息存放在内核中，可以选择删除队列
    if(msgctl(msqid,IPC_RMID,NULL)==-1){
        perror("msgctl rm failed");
        return -1;
    }
    return 0;
}