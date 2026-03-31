#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>
#include <sys/wait.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
    pid_t pid=fork();
    if(pid==-1){
        perror("fork failed");
        return 0;
    }
    //子进程从fifo2中读取数据
    else if(pid==0){
        int fd=-1;
        if((fd=open("./fifo2",O_RDONLY))==-1){
            perror("open failed");
            return -1;
        }

        char rbuf[128]="";
        while(1){
            //清空容器，补0
            memset(rbuf,0,sizeof(rbuf));
            read(fd,rbuf,sizeof(rbuf));
            printf("%s\n",rbuf);
            if(strcmp(rbuf,"quit")==0)break;
        }

        close(fd);
        exit(0);
    }
    //父进程向fifo1中写入数据
    else{
        int fd=-1;
        if((fd=open("./fifo1",O_WRONLY))==-1){
            perror("open failed");
            return -1;
        }
        char wbuf[128]="";
        while(1){
            printf("please input>>>");
            fgets(wbuf,sizeof(wbuf),stdin);
            wbuf[strlen(wbuf)-1]=0;
            write(fd,wbuf,strlen(wbuf));
            if(strcmp(wbuf,"quit")==0)break;
        }
        close(fd);
        wait(NULL);
    }
}