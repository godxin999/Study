/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/6 18:28:35
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
//实例化函数模板
template <typename T>
int compare1(const T &v1,const T &v2){//泛型代码的第一个原则，函数参数为const的引用
	if(v1<v2)return -1;//泛型代码的第二个原则，条件判断仅使用<比较运算
	if(v2<v1)return 1;
	return 0;
}
void test1(){
	cout<<compare1(0,1)<<endl;//实例化为int compare(const int&,const int&);
	vector<int> v1{1,2,3},v2{4,5,6};
	cout<<compare1(v1,v2)<<endl;//实例化为int compare(const vector<int>&,const vector<int>&);
}
//模板类型参数
template <typename T> T foo(T *p){
	T temp=*p;
	return temp;
}
void test2(){
	int a=100;
	int *p=&a;
	cout<<foo(p)<<endl;
}
//在在模板参数列表中，class和typename没有区别
template <class T,typename U> T calc(const T&,const U&);
//非类型模板参数
template <unsigned N,unsigned M>//实例化的时候会被用户提供的值或者编译器推断出的值所替代，这些值必须为常量表达式
int compare2(const char (&p1)[N],const char (&p2)[M]){//比较两个字符数组，两个模板参数分别为两个数组的长度
	return strcmp(p1,p2);
}
void test3(){
	cout<<compare2("hi","mom")<<endl;//编译器通过字面常量的大小来代替N和M，其中编译器会插入一个空字符作为结束，所以该函数会实例化为int compare(const char (&p1)[3],const char (&p2)[4]);
}
//函数模板可以被声明为inline或constexpr，说明符放在模板参数列表之后，返回类型之前
template <typename T> inline T min(const T&,const T&);
//类型无关，可移植的compare函数，使用less实现的小于
template <typename T> int compare(const T &v1,const T &v2){
	if(less<T>()(v1,v2))return -1;
	if(less<T>()(v2,v1))return 1;
	return 0;
}
void test4(){
	int a=100,b=200;
	int *p1=&a,*p2=&b;
	cout<<compare(p1,p2)<<endl;
}
int main(){
	//test1();
	//test2();
	//test3();
	test4();


}

