#include<iostream>
#include<stdio.h>
using namespace std;

int main(){
    printf("未使用缓冲区时，行缓存为：%d\n",stdout->_IO_buf_end-stdout->_IO_buf_base);
    printf("使用缓冲区之后，行缓存为：%d\n",stdout->_IO_buf_end-stdout->_IO_buf_base);

    printf("未使用缓冲区时，行缓存为：%d\n",stdin->_IO_buf_end-stdin->_IO_buf_base);
    int num=0;
    cin>>num;
    printf("使用缓冲区之后，行缓存为：%d\n",stdin->_IO_buf_end-stdin->_IO_buf_base);

    FILE*fp=NULL;
    if((fp=fopen("./file.txt","r"))==NULL){
        perror("error");
        return -1;
    }
    printf("未使用缓冲区，全缓冲为：%d\n",fp->_IO_buf_end-fp->_IO_buf_base);
    fgetc(fp);
    printf("使用缓冲区之后，全缓冲为：%d\n",fp->_IO_buf_end-fp->_IO_buf_base);
    return 0;
}