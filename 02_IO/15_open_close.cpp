#include<iostream>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
using namespace std;

int main(){
    int fp=-1;
    //以只读方式打开文件，如果没有文件，创建新文件，并给定文件的权限
    if((fp=open("../open.txt",O_RDONLY|O_CREAT,0644))==-1){
        perror("error");
        return -1;
    }
    cout<<"success"<<endl;
    cout<<"打开的文件的文件描述符为："<<fp<<endl;

    close(fp);

    return 0;
}