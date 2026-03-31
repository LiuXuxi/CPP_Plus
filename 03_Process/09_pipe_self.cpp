#include <sys/types.h>
#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

//管道可以自己跟自己通信

int main(){
    int fds[2];
    pipe(fds);
    
    char wbuf[20]="hello world";
    write(fds[1],wbuf,strlen(wbuf));

    char rbuf[20]="";
    read(fds[0],rbuf,sizeof(rbuf));

    printf("%s\n",rbuf);

    return 0;
}