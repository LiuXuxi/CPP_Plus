#pragma once


//创建一个信号量集
//@semcoutn:创建的信号量个数
int create_sem(int semcount);

//申请一个指定的信号量
//@semid:信号量id
//@semno:申请的信号量
int P(int semid,int semno);

//释放一个指定的信号量
//@semid:信号量id
//@semno:释放的信号量
int V(int semid,int semno);

//删除一个指定的信号量集
//@semid:信号量id
int delete_sem(int semid);
