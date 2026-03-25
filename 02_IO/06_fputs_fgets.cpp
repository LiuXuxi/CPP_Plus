#include"stdio.h"
#include<iostream>
#include<string.h>

int main(){
    FILE* fp=NULL;
    if((fp=fopen("./file.txt","w"))==NULL){
        perror("fopen failed:");
        return -1;
    }
    char wbuf[128]="";
    while(1){
        std::cin>>wbuf;
        if(strcmp(wbuf,"quit")==0)break;
        fputs(wbuf,fp);
        fputs("\n",fp);
    }
    fclose(fp);

    if((fp=fopen("./file.txt","r"))==NULL){
        perror("fopen failed:");
        return -1;
    }
    
    char rbuf[128]="";
    while(1){
        char*res=fgets(rbuf,sizeof(rbuf),fp);
        if(res==NULL)break;
        printf("%s",rbuf);
    }
    fclose(fp);
    return 0;
}