#include "../include/myswap.h"
#include<iostream>

myswap::myswap(int a,int b)
{
    this->a=a;
    this->b=b;
}

void myswap::swap(){
    int temp=a;
    a=b;
    b=temp;
}

void myswap::println(){
    std::cout<<"a="<<a<<std::endl;
    std::cout<<"b="<<b<<std::endl;
}

