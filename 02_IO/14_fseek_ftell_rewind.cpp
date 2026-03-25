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

    //把光标偏移到第二个学生位置
    fseek(fp,sizeof(stu),SEEK_SET);
    //读取第二个学生的信息
    fread(&student,sizeof(stu),1,fp);
    printf("第二名学生信息为：%s %d %.2lf\n",student.name,student.age,student.height);
    
    //把光标移到文件开头
    rewind(fp);
    //读取第一个学生的信息
    fread(&student,sizeof(stu),1,fp);
    printf("第一名学生信息为%s %d %.2lf\n",student.name,student.age,student.height);

    //把光标移到文件末尾
    fseek(fp,0,SEEK_END);
    //读取文件大小
    printf("文件大小为:%ld\n",ftell(fp));

    fclose(fp);

    return 0;
}