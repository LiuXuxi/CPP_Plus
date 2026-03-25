#include"stdio.h"

int main(){
    FILE*fp=NULL;
    if((fp=fopen("./file.txt","w"))==NULL){
        perror("fopen failed:");
        return -1;
    }
    fputc('H',fp);
    fputc('e',fp);
    fputc('l',fp);
    fputc('l',fp);
    fputc('o',fp);

    fclose(fp);

    if((fp=fopen("./file.txt","r"))==NULL){
        perror("fopen failed:");
        return -1;
    }

    char c=0;
    while(1){
        c=fgetc(fp);
        if(c==EOF)break;
        printf("%c ",c);
    }
    printf("\n");
    fclose(fp);

    return 0;
}