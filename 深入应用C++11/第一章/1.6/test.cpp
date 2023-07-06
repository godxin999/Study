/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/7/6 20:53:21
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <functional>
using namespace std;
//没有参数的时候，lambda表达式可以省略参数列表
void test1(){
	auto f=[]{return 1;};
	cout<<f()<<endl;
}
//如果需要修改值捕获变量的副本，需要声明mutable
void test2(){
	int a=100;
	/*
	auto f=[=](){
		return ++a;//error，修改值捕获外部变量
	};
	*/
	auto f2=[=]()mutable{//声明为mutable的lambda表达式不能省略参数列表
		return ++a;
	};
	cout<<f2()<<" "<<f2()<<endl;//101 102
	cout<<a<<endl;//100
}
//使用function和bind存储lambda表达式
void test3(){
	function<int(int)> f1=[](int a){return a;};
	function<int(void)> f2=std::bind([](int a){return a;},123);
	cout<<f1(100)<<" "<<f2()<<endl;//100 123
}
//没有捕获变量的lambda表达式可以被转换为一个普通的函数指针
void test4(){
	using func_t=int(*)(int);
	func_t f=[](int a){return a;};
	cout<<f(123)<<endl;
	typedef void(*ptr)(int*);
	ptr p=[](int* p){delete p;};
	//ptr p1=[&](int* p){delete p;};//错误，有状态的lambda表达式不能转换为函数指针
}
int main(){
	//test1();
	//test2();
	//test3();
	test4();


}

