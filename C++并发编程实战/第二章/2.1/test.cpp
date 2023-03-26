/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/26 18:38:13
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <thread>
#include <cassert>
using namespace std;
//启动线程
void do_something(){}
void do_something(int i){}
void do_something_else(){}
void do_something_in_current_thread(){}
class background_task{
public:
	void operator()()const{
		cout<<"thread open"<<endl;
		do_something();
		do_something_else();		
	}
};
void test1(){
	background_task f;
	thread mysthread(f);//将重载了调用运算符的类型的对象传给thread类，替换默认的构造函数
	//thread mythread2(background_task());//如果传入一个临时变量，会解析为一个函数声明，这个函数有一个参数，并且返回一个thread对象
	thread mythread3((background_task()));//可以通过使用多组括号和使用大括号进行初始化来避免上述问题
	thread mythread4{background_task()};
	thread mythread5([](){//使用lambda表达式可以避免传递参数
		do_something();
		do_something_else();
	});
}
//避免使用可以访问局部变量的函数对象创建线程
struct func{
	int &i;
	func(int &i_):i(i_){}
	void operator()(){
		cout<<"thread open"<<endl;
		for(unsigned j=0;j<1e6;++j){
			do_something(i);//存在访问隐患，悬空引用
		}
	}
};
void test2(){
	int some_local_state=0;
	func myfunc(some_local_state);
	thread mythread(myfunc);//使用一个可以访问局部变量的函数去创建线程是危险的
	//mythread.detach();//不等待线程结束
	mythread.join();//可以使用join()函数来保证线程在函数完成前结束
}//函数结束后新线程可能还在运行，会访问已经销毁的局部变量
//在异常处理中调用join()函数
void test3(){
	int some_local_state=0;
	func myfunc(some_local_state);
	thread mythread(myfunc);
	try{//防止线程在调用join()之前抛出异常
		do_something_in_current_thread();
	}
	catch(...){//如果抛出异常
		mythread.join();//在异常处理中使用join()函数
		throw;
	}
	mythread.join();
}
//RAII(资源获取即初始化)，在析构函数中调用join()函数
class thread_guard{
	thread &t;
public:
	explicit thread_guard(thread &t_):t(t_){}
	~thread_guard(){
		if(t.joinable()){
			t.join();
		}
	}
	thread_guard(const thread_guard&)=delete;
	thread_guard& operator=(const thread_guard&)=delete;
};
void test4(){
	int some_local_state=0;
	func myfunc(some_local_state);
	thread t(myfunc);
	thread_guard g(t);
	do_something_in_current_thread();
}
//使用detach分离线程
void test5(){
	thread t(do_something_else);
	t.detach();
	assert(!t.joinable());
}
//使用分离线程来处理其他文档
/*
void edit_document(const string &filename){
	open_document(filename);
	while(!done_editing()){
		user_command cmd=get_user_input();
		if(cmd.type==open_new_document){
			const string new_name=get_file_name_from_user();
			thread t(edit_document,new_name);//启动新线程去打开新文档
			t.detach();//分离线程
		}
		else{
			process_user_input(cmd);
		}
	}
}
*/
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	test5();
}

