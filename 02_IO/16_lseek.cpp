#include<iostream>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>
using namespace std;

int main(){
    int fd=-1;
    if((fd=open("../test.txt",O_WRONLY|O_CREAT|O_TRUNC,0644))==-1){
        perror("open error");
        return -1;
    }

    char wbuf[128]="hello world";
    write(fd,wbuf,strlen(wbuf));

    close(fd);

    if((fd=open("../test.txt",O_RDONLY))==-1){
        perror("open error");
        return -1;
    }

    //光标移动到第五个字节
    lseek(fd,5,SEEK_SET);
    char rbuf[128];
    read(fd,rbuf,5);
    cout<<rbuf<<endl;

    //光标移到最后，并查看文件大小
    int size=lseek(fd,0,SEEK_END);
    cout<<"文件大小为："<<size<<endl;

    close(fd);

    return 0;
}