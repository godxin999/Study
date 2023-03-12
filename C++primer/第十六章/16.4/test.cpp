/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/12 22:29:40
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//可变参数模板
template<typename T,typename... Args>//Args是模板参数包
void foo(const T&,const Args& ... rest){}//rest是函数参数包
void test1(){
	int i=0;
	double d=3.14;
	string s="how now brown cow";
	foo(i,s,42,d);//实例化为foo(const int&,const string&,const int&,const double&);
	foo(s,42,"hi");//实例化为foo(const string&,const int&,const char[3]&);
	foo(d,s);//实例化为foo(const double&,const stirng&);
	foo("hi");//实例化为foo(const char[3]&);
}
//sizeof...运算符获取参数包的参数数量
template <typename ...Args>
void g(Args ... args){
	cout<<sizeof...(Args)<<" ";//输出类型参数的数目
	cout<<sizeof...(args)<<endl;//输出函数参数的数目
}
void test2(){
	g(1,2,3);//3 3
	g(1,'a');//2 2

}

int main(){
	//test1();
	test2();




}

