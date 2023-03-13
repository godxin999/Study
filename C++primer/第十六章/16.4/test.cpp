/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/12 22:29:40
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <sstream>
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
//可变参数函数通常是递归的，需要定义一个非可变参数函数来终止递归
template <typename T>
ostream& print(ostream& os,const T &t){//如果不定义非可变参数模板，那么可变参数版本会进行无限递归
	return os<<t;
}
template <typename T,typename... Args>
ostream& print(ostream& os,const T &t,const Args&... rest){
	os<<t<<" ";
	return print(os,rest...);
}
void test3(){
	string s="100";
	int i=6;
	print(cout,i,s,42);//6 100 42
}
//包扩展
template <typename T>
string debug_rep(const T &t){
	ostringstream ret;
	ret<<t;
	return ret.str();
}
template <typename T>
string debug_rep(T *p){
	ostringstream ret;
	ret<<"pointer: "<<p;
	if(p){
		ret<<" "<<debug_rep(*p);
	}
	else ret<<" null pointer";
	return ret.str();
}
template <typename... Args>
ostream& errorMsg(ostream &os,const Args&... rest){
	return print(os,debug_rep(rest)...);//对rest中的每个元素调用debug_rep
	//return print(os,debug_rep(rest...));//错误，扩展rest等价于调用接受可变参数的debug_rep
}
void test4(){
	errorMsg(cerr,1,"a",3.14);//1 pointer: a a 3.14
}
int main(){
	//test1();
	//test2();
	//test3();
	test4();
}

