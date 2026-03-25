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

    int num=18;
    fwrite(&num,sizeof(num),1,fp);

    fclose(fp);

    if((fp=fopen("../test.txt","r"))==NULL){
        perror("error");
        return -1;
    }

    int nnum=0;
    fread(&nnum,sizeof(nnum),1,fp);

    printf("%d\n",nnum);

    fclose(fp);

    return 0;
}