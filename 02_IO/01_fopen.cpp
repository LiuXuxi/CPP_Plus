#include "stdio.h"

int main(){
    FILE*fp=NULL;
    fp=fopen("./file1.txt","r");
    if(fp==NULL){
        printf("fopen error by mode r\n");
    }
    else printf("fopen succeed by mode r\n");
    fp=fopen("./file2.txt","w");
    if(fp==NULL)printf("fopen error by mode w\n");
    else printf("fopen succeed by mode w\n");
    return 0;
}