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

        // //防止关闭读端，用来验证写端
        // while(1);

        //关闭读端
        close(fds[0]);
    }
    //父进程向管道写数据
    else{
        //父进程写时关闭读端
        close(fds[0]);
        
        //一直向管道内写入数据，会发生什么？
        char w='a';
        int cnt=0; //统计写入的字节数
        while(1){
            write(fds[1],&w,1);
            ++cnt;
            //记得刷新缓冲区，否则循环内无法看到输出
            printf("%d\n",cnt);
        }

        //可以看到，cnt=65535，也就是说
        //当读端存在时，管道缓冲区最多写入64K数据，然后阻塞
        //当读端不存在时，再写入则直接管道破裂，内核空间向用户空间发射一个SIGPIPE信号
        //进程收到这个信号后，退出

        //写完后关闭写端
        close(fds[1]);
    }
    return 0;
}