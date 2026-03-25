#include<iostream>
#include<stdio.h>
using namespace std;

int main(){
    // //验证只有遇到时机才刷新缓冲区
    // printf("hello world");
    // while(1);

    // //当程序结束时，刷新缓冲区
    // printf("hello world");
    // return 0;

    // //当遇到\n时刷新
    // printf("hello world\n");
    //while(1);

    // //当切换输入输出模式时刷新
    // cout<<"hello world"<<endl;
    // int num;
    // cin>>num;
    //while(1);

    // //当关闭行缓存的文件指针时，刷新
    // //行缓存文件指针默认打开
    // cout<<"hello world"<<endl;
    // fclose(stdout);
    //while(1);

    // //手动刷新缓冲区
    // cout<<"hello world";
    // fflush(stdout);
    //while(1);

    // //缓冲区满了时会刷新,多余的内容不会刷新，放入刷新后的缓冲区
    // for(int i=0;i<3000;i++)cout<<"a";
    // while(1);

    return 0;
}