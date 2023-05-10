/*************************************************************************
	> File Name: item24.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/10 21:00:35
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <type_traits>
using namespace std;
class widget{};
//通用引用
//1.函数模板参数
template <typename T>
void f(T&& param){
	if(is_same<decltype(std::forward<decltype(param)>(param)),widget&&>::value){
		cout<<"widget&&"<<endl;
	}
	else if(is_same<decltype(std::forward<decltype(param)>(param)),widget&>::value){
		cout<<"widget&"<<endl;
	}
}
//2.auto声明
widget w1;
auto&& w2=w1;
//右值引用
void f1(widget&& w){}//没有类型推导，是右值引用
widget&& w=widget();//右值引用
//引用折叠
void test1(){
	widget w;
	f(w);//widget&
	f(std::move(w));//widget&&
}
//vector里的引用
template <class T,class Allocator=allocator<T>>
class vector{
public:
	void push_back(T&& x);//右值引用
	template <class... Args>
	void emplace_back(Args&&... args);//通用引用
};
//auto的变量可以是通用引用
void test2(){
	auto timeFuncInvocation=[](auto&& func,auto&&... params){//C++14
		std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)...);//使用函数func调用参数params
	};
}



int main(){
	//test1();
	test2();




}