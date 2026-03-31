#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>
#include<unistd.h>

int main(){
    int fd=-1;
    if((fd=open("./fifo",O_RDONLY))==-1){
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
    return 0;
}