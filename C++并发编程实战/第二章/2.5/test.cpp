/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/2 17:59:22
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <thread>
using namespace std;
//使用get_id()成员函数获取线程标识符
void do_something(){}
void do_something_else(){}
void test1(){
	thread mythread(do_something);
	cout<<mythread.get_id()<<endl;//输出对应线程的标识符
	mythread.join();
	thread t;
	cout<<t.get_id()<<endl;//0，因为t未执行任何线程关联
	cout<<this_thread::get_id()<<endl;//获取当前线程的线程标识
}
//使用thread::id检查线程是否需要进行一些操作
std::thread::id master_thread;
void some_core_part_of_algorithm(){
	if(std::this_thread::get_id()==master_thread){
		do_something();
	}
	do_something_else();
}
int main(){
	test1();
}