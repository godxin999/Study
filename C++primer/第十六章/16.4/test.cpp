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
//模板特例化
template <typename T>
int compare(const T &a,const T &b){//可以比较任意两个类型
	return a<b;
}
template <size_t N,size_t M>
int compare(const char (&p1)[N],const char (&p2)[M]){//可以处理字符串字面常量
	return strcmp(p1,p2);
}
template<>//空尖括号指出我们将要为原模版的所有模板参数提供实参，需要原模版的声明在作用域中
int compare(const char* const &p1,const char* const &p2){//特例化模板，处理字符数组的指针
	return strcmp(p1,p2);
}
void test5(){
	const char *p1="hi",*p2="mom";
	cout<<compare(p1,p2)<<endl;
	cout<<compare("hi","mom")<<endl;

}
//特例化成员而不是类
template <typename T>
struct Foo{
	Foo(const T &t=T()):mem(t){}
	void Bar(){
		cout<<"正常实例化模板"<<endl;
	}
	T mem;
};
template <>
void Foo<int>::Bar(){
	cout<<"特例化Foo<int>::Bar"<<endl;
}
void test6(){
	Foo<string> fs;
	fs.Bar();//正常实例化模板
	Foo<int> fi;
	fi.Bar();//特例化Foo<int>::Bar
}

int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	test6();
}

