#include<iostream>
#include<fstream>
#include<string>
using namespace std;

void test01(){
    //输出流，输出程序内容到文件中
    //创建对象时打开文件
    //以追加的形式打开文件
    ofstream ofd("../stream.txt",ios::app);
    
    if(ofd.is_open()){
        //输出内容到文件中，直接用输出流运算符
        ofd<<"hello ofstream"<<endl;
    }else cerr<<"open faile"<<endl;

    //关闭文件
    ofd.close();

    //输入流，将文件的内容输入到程序中
    ifstream ifd("../stream.txt");

    if(!ifd.is_open())cerr<<"open failed"<<endl;

    string line;//用字符串接受从文件输入的内容
    //读取文件全部内容
    while(getline(ifd,line))cout<<line<<endl;

    //关闭文件
    ifd.close();

}

struct student{
    char name[20];
    int age;
    double height;
};

void test02(){
    //以二进制形式打开文件进行输出
    ofstream ofd("../student.txt",ios::out|ios::binary);
    if(!ofd.is_open()){
        cerr<<"open failed"<<endl;
        return;
    }

    struct student s1{"xuedu",18,1.78};
    ofd.write((const char*)&s1,sizeof(struct student));

    //永远不要忘了关闭文件
    ofd.close();

    //以二进制形式打开文件进行输入
    ifstream ifd;
    ifd.open("../student.txt",ios::in|ios::binary);
    if(!ifd.is_open()){
        cerr<<"open failed"<<endl;
        return;
    }

    struct student s2;
    ifd.read((char*)&s2,sizeof(struct student));
    cout<<s2.name<<" "<<s2.age<<" "<<s2.height<<endl;

    ifd.close();

}

int main(){
    //test01();
    test02();
    return 0;
}