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

    //3.发送数据
    msgBuf buf;
    while(1){
        printf("请输入消息类型：");
        scanf("%ld",&buf.mtype);
        //吸收回车，防止回车称为消息正文
        getchar();
        printf("请输入消息正文：");
        //输入正文
        fgets(buf.mtext,MSGSZ,stdin);
        //把回车替换为结束符0
        buf.mtext[strlen(buf.mtext)-1]=0;
        //发送数据
        //最多发送16K数据，当队列满时阻塞输入
        if(msgsnd(msqid,&buf,MSGSZ,0)==-1){
            perror("msgsnd failed");
            return -1;
        }
        //当输入quit时退出循环停止后续输入
        if(strcmp(buf.mtext,"quit")==0)break;
    }
    return 0;
}