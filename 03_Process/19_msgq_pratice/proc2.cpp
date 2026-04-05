#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include<string.h>
#include <sys/wait.h>
#include<stdlib.h>
#define MSGSZ sizeof(msgBuf)-sizeof(long)

typedef struct msgBuf{
    long mtype;
    char mtext[1024];
}msgBuf;

int main(){
    //创建消息队列的key值
    int key=ftok("/",'k');
    if(key==-1){
        perror("ftok failed");
        return -1;
    }
    printf("key is %x\n",key);

    //创建消息队列
    int msqid=-1;
    if((msqid=msgget(key,IPC_CREAT|0644))==-1){
        perror("msgget failed");
        return -1;
    }
    printf("msgid is %d\n",msqid);

    //创建子进程
    pid_t pid=-1;
    if((pid=fork())==-1){
        perror("fork failed");
        return -1;
    }
    //子进程向消息队列发送类型为2的消息
    else if(pid==0){
                msgBuf buf;
        while(1){
            //输入消息类型
            printf("please input msg type>>>");
            scanf("%ld",&buf.mtype);
            getchar();
            //输入消息正文
            printf("please input msg text>>>");
            fgets(buf.mtext,MSGSZ,stdin);
            buf.mtext[strlen(buf.mtext)-1]=0;

            //发送消息
            if((msgsnd(msqid,&buf,MSGSZ,0))==-1){
                perror("msgsnd failed");
                return -1;
            }

            if(strcmp(buf.mtext,"quit")==0)break;

        }
        wait(NULL);
    }
    //父进程从消息队列取类型为1的消息
    else {
        msgBuf buf;
        //接受消息
        while(1){
            //清空消息容器，防止错误输出
            memset(buf.mtext,0,sizeof(buf.mtext));
            if((msgrcv(msqid,&buf,MSGSZ,1,0))==-1){
                perror("msgrcv failed");
                return -1;
            }

            printf("rcv msg: %s\n",buf.mtext);
            
            if(strcmp(buf.mtext,"quit")==0)break;
        }
        exit(0);
    }
    return 0;
}