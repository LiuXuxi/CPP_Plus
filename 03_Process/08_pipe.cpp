#include <sys/types.h>
#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(){
    //存储文件描述符，0是读端，1是写端
    int fds[2];
    //创建无名管道 
    //为了保证父子进程公用一个管道，必须要在fork之前创建管道
    pipe(fds);

    pid_t pid=fork();
    if(pid<0){
        perror("fork failed");
        return -1;
    }
    //子进程向管道读数据
    else if(pid==0){
        //子进程读时关闭写端
        close(fds[1]);

        //向读端读取数据
        char rbuf[20]="";
        read(fds[0],rbuf,sizeof(rbuf));

        //打印读取的数据
        printf("%s\n",rbuf);

        //关闭读端
        close(fds[0]);
    }
    //父进程向管道写数据
    else{
        //父进程写时关闭读端
        close(fds[0]);
        
        //向管道写入数据
        char wbuf[20]="hello world";
        write(fds[1],wbuf,strlen(wbuf));

        //写完后关闭写端
        close(fds[1]);
    }
    return 0;
}