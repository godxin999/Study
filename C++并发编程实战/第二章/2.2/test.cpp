/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/28 23:04:51
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <thread>
#include <memory>
using namespace std;
//向线程函数中传递参数
void f(int i,const string &s){
	cout<<"f(int,const string&) is called"<<endl;
}
void test1(){
	thread t(f,3,"hello");
}
void test2(int some_param){
	char buffer[1024];//buffer指向局部变量
	sprintf(buffer,"%i",some_param);
	//thread t(f,3,buffer);//本地变量通过buffer传入新线程中，函数可能在字符串字面值隐式转换为string之前退出，从而导致未定义的操作
	thread t(f,3,static_cast<string> (buffer));//正确做法，提前进行转换
	//t.join();
	t.detach();
}
/*
void update_data_for_widget(widget_id w,widget_data &data);
void test3(widget_id w){
	widget_data data;
	//thread t(update_data_for_widget,w,data);//我们希望第二个参数传递一个引用，但是thread的构造函数并不知道，所以它会对盲目拷贝给定的值，但是在代码中将拷贝的值以右值进行传递，我们的参数要求一个非常量引用，因为非常量引用不能绑定到右值上，所以代码会报错
	thread t(update_data_for_widget,w,std::ref(data));//正确做法，使用std::ref传递一个引用
	display_status();
	t.join();
	process_widget_data(data);
}
*/
//使用成员函数指针作为线程函数，并选择合适的对象指针作为第一个参数
class X{
public:
	void do_lengthy_work(){}
};
void test3(){
	X my_X;
	thread t(&X::do_lengthy_work,&my_X);
}
//使用unique_ptr将一个动态对象转移到一个线程中去
/*
void process_big_object(unique_ptr<big_object>);
void test4(){
	unique_ptr<big_object> p(new big_object);
	p->prepare_data(42);
	thread t(process_big_object,std::move(p));
}
*/
int main(){
	//test1();
	//test2(10);
	test3();

}