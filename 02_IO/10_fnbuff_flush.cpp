#include<iostream>
#include<stdio.h>
using namespace std;

int main(){
    // //向标准出错中放入数据，立马刷新
    // perror("error");
    // while(1);

    //向标准出错缓冲区写入内容，立马刷新
    fputs("abc",stderr);
    while(1);

    return 0;
}