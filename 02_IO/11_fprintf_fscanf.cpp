#include<stdio.h>
#include<string>
using namespace std;

int main(){
    //向标准输出中写入内容
    fprintf(stdout,"%s %d\n","xuedu",12345);

    //向标准输入中读出内容
    char userName[20];
    int pwd;
    fscanf(stdin,"%s %d",userName,&pwd);
    printf("%s %d\n",userName,pwd);

    //向标准错误中读出内容
    int error;
    fscanf(stderr,"%d",error);
    printf("%d\n",error);

    //从外部文件中写入内容
    FILE* fp=NULL;
    if((fp=fopen("../usr.txt","w"))==NULL){
        perror("error");
        return -1;
    }

    fprintf(fp,"%s %d","xuedu",123456);

    fclose(fp);

    //从外部文件中读出内容
    if((fp=fopen("../usr.txt","r"))==NULL){
        perror("error");
        return -1;
    }

    fscanf(fp,"%s %d",userName,&pwd);

    fclose(fp);

    printf("%s %d\n",userName,pwd);

    return 0;
}