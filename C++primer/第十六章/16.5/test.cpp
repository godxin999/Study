/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/13 13:11:51
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

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
void test1(){
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
void test2(){
	Foo<string> fs;
	fs.Bar();//正常实例化模板
	Foo<int> fi;
	fi.Bar();//特例化Foo<int>::Bar
}
int main(){
	//test1();
	test2();
}