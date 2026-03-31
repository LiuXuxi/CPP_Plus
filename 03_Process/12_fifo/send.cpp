#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>
#include<unistd.h>

int main(){
    int fd=-1;
    if((fd=open("./fifo",O_WRONLY))==-1){
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
    return 0;
}