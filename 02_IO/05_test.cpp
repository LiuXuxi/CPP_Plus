#include"stdio.h"

int main(int argc,const char *argv[]){
    if(argc!=3){
        printf("input file error\n");
        printf("usage:./a.out srcfile dest file\n");
        return -1;
    }
    FILE*srcfp=NULL,*destfp=NULL;
    if((srcfp=fopen(argv[1],"r"))==NULL){
        perror("fopen scr failed:");
        return -1;
    }
    if((destfp=fopen(argv[2],"w"))==NULL){
        perror("fopen dest failed:");
        return -1;
    }

    char c=0;
    while(1){
        c=fgetc(srcfp);
        if(c==EOF)break;
        fputc(c,destfp);
    }

    printf("copy succeed\n");

    fclose(srcfp);
    fclose(destfp);
    
    return 0;
}