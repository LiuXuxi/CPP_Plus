#include<thread>
#include<iostream>
#include<string>

// //1.无返回值无参函数
// void task1(){
//     std::cout<<"this thread1 is "<<std::this_thread::get_id()<<std::endl;
// }

// //2.无返回值有参函数
// void task1(int num,std::string s){
//     std::cout<<"num is "<<num<<" s is "<<s<<std::endl;
// }

//3.类函数/仿函数
class student{
public:
    void task1(int num,std::string s){
        std::cout<<"num is "<<num<<"s is "<<s<<std::endl;
    }

};

int main(){
    //创建线程类
    std::string s{"hello thread"};
    student stu;
    std::thread th1(&student::task1,&stu,10,s);

    //4.lambda表达式
    std::thread th2([](int num,std::string s){
        std::cout<<"num is "<<num<<"s is "<<s<<std::endl;
    },10,s);

    //等待线程回收
    th1.join();
    th2.join();

    return 0;
}