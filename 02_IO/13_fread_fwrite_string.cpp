#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;

int main(){
    FILE*fp=NULL;
    if((fp=fopen("../test.txt","w"))==NULL){
        perror("error");
        return -1;
    }

    char wbuf[20]="";
    
    while(1){
        fgets(wbuf,sizeof(wbuf),stdin);
        if(strcmp(wbuf,"quit\n")==0)break;
        fwrite(wbuf,strlen(wbuf),1,fp);
        fflush(fp);
    }

    fclose(fp);

    if((fp=fopen("../test.txt","r"))==NULL){
        perror("error");
        return -1;
    }

    char rbuf[20];
    //fread(rbuf,sizeof(rbuf),1,fp);
    fread(rbuf,sizeof(char),sizeof(rbuf),fp);

    printf("%s",rbuf);

    fclose(fp);

    return 0;
}