#include<iostream>
#include<stdio.h>
using namespace std;

class stu{
public:
    char name[20];
    int age;
    double height;
};

int main(){
    FILE*fp=NULL;
    if((fp=fopen("../test.txt","w"))==NULL){
        perror("error");
        return -1;
    }

    stu s[3]{{"张三",18,1.78},{"李四",20,1.80},{"王五",19,1.77}};
    fwrite(s,sizeof(stu),3,fp);

    fclose(fp);

    if((fp=fopen("../test.txt","r"))==NULL){
        perror("error");
        return -1;
    }

    stu student;
    fread(&student,sizeof(stu),1,fp);
    printf("%s %d %.2lf\n",student.name,student.age,student.height);

    fclose(fp);

    return 0;
}