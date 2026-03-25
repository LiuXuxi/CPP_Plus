#include<iostream>
#include<stdio.h>
using namespace std;

int main(){
    FILE*fp=NULL;
    if((fp=fopen("../a.txt","w"))==NULL){
        perror("error");
        return -1;
    }

    // //验证当没有遇到时机，不会刷新缓存区
    // fputs("hello world",fp);
    // while(1);

    // //当遇到换行时，也不会刷新缓存区
    // fputs("hello world\n",fp);
    // while(1);

    // //当程序结束时会刷新缓存区
    // fputs("hello world",fp);
    // return 0;

    // //当输入输出模式切换时刷新
    // fputs("hello",fp);
    // fgetc(fp);
    // while(1);

    // //当关闭全缓冲的文件指针时也会刷新
    // fputs("hea;f",fp);
    // fclose(fp);
    // while(1);

    // //手动刷新文件指针，也会刷新
    // fputs("dfasaeif",fp);
    // fflush(fp);
    // while(1);

    // //缓冲区满了也会刷新
    // for(int i=0;i<4097;i++)fputc('a',fp);
    // while(1);

    fclose(fp);

    return 0;
}