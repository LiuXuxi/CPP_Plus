#include<iostream>
#include"./include/myswap.h"

int main(){
    myswap my(10,20);
    std::cout<<"swap before:"<<std::endl;
    my.println();
    my.swap();
    std::cout<<"swap after:"<<std::endl;
    my.println();
    return 0;
}