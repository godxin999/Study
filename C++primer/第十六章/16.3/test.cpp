/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/12 17:25:58
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <sstream>
using namespace std;
//重载模板
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
void test1(){
	string s("hi");
	cout<<debug_rep(s)<<endl;//实例化第一个模板
	cout<<debug_rep(&s)<<endl;//两个模板均能实例化，第一个实例化为debug_rep(const string*&)，第二个实例化为debug_rep(string*)，因为第一个模板需要进行普通指针到const指针的转换，所以匹配规则选择匹配第二个模板
}
void test2(){
	string s("hi");
	const string *sp=&s;
	cout<<debug_rep(sp)<<endl;//两个模板均能实例化，因为第一个模板debug_rep(const T&)本质上可以用于任何类型，包括指针类型，所以这条调用会被解析为debug_rep(T*)，即更特例化的版本。
}
string debug_rep(const string &s){
	return '"'+s+'"';
}
void test3(){
	string s("hi");
	cout<<debug_rep(s)<<endl;//"hi"，对于一个调用，如果一个非函数模板与一个函数模板提供同样好的匹配，则选择非模板版本
}
//重载模板和函数匹配
void test4(){
	cout<<debug_rep("hi world!")<<endl;//三个模板均可以实例化，第一个模板T被绑定到char[10]，第二个模板T被绑定到const char，第三个模板要求从const char*到string的类型转换，根据匹配原则，非模板函数需要一次类型转换，不是最优，在两个函数模板中，第二个更加特例化，所以选择第二个函数模板
}
string debug_rep(char *p){//定义两个非模板重载版本将字符指针按string处理
	return debug_rep(string(p));//为了不调用错误版本的debug_rep，需要声明string debug_rep(const string &)，否则就会实例化第一个模板
}
string debug_rep(const char *p){
	return debug_rep(string(p));//如果定义这个函数，test4将会输出"hi world!"
}

int main(){
	//test1();
	//test2();
	//test3();
	test4();
}