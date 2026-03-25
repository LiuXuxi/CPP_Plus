#include"stdio.h"
#include<iostream>
#include<string.h>

int main(){
    FILE* srcfp=NULL,*destfp=NULL;
    if((srcfp=fopen("./test1.txt","r"))==NULL){
        perror("fopen src failed:");
        return -1;
    }
    if((destfp=fopen("./test2.txt","w"))==NULL){
        perror("fopen dest failed:");
        return -1;
    }
    
    char rbuf[128]="";
    while(1){
        char*res=fgets(rbuf,sizeof(rbuf),srcfp);
        if(res==NULL)break;
        fputs(rbuf,destfp);
    }

    printf("copy succeed\n");
    
    fclose(srcfp);
    fclose(destfp);
    return 0;
}