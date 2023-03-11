/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/11 14:45:13
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <vector>
#include <type_traits>
using namespace std;

//在模板中可以应用的类型转换只有const转换和函数或数组到指针的转换
template <typename T>
void fobj(T,T){}
template <typename T>
void fref(const T&,const T&){}
void test1(){
	string s1("a value");
	const string s2("another value");
	fobj(s1,s2);//调用fobj(string,string)，const被忽略
	fref(s1,s2);//调用fref(const string&,const string&)，将s1转换为const是允许的
	int a[10],b[42];
	fobj(a,b);//调用f(int*,int*)
	//fref(a,b);//错误，数组类型不匹配
}
//如果希望函数实参进行正常的类型转换，我们可以将函数模板定义为两个类型参数
template <typename A,typename B>
int flexibleCompare(const A& v1,const B& v2){
	if(v1<v2)return 1;//需要提前定义<运算符
	if(v2<v1)return -1;
	return 0;
}
void test2(){
	long lng=0;
	flexibleCompare(lng,1024);
}
//如果函数参数不是模板参数，那实参会进行正常的类型转换
template <typename T>
ostream& print(ostream &os,const T &obj){
	return os<<obj;
}
void test3(){
	print(cout,42);
	ofstream f;
	print(f,10);//f会隐式转换为ostream&
}
//指定函数模板显式实参
template <typename T1,typename T2,typename T3>
T1 sum(T2,T3){
	return 1;
}
void test4(){
	int i=19;
	long long l=100;
	auto val=sum<long long>(i,l);//T1显式指定为long long，T2和T3是从函数实参类型中推断出来的
	auto val2=sum<long long,int,long long>(i,l);//显式指定所有类型
}
//对于模板类型参数已经显式指定的函数实参，可以进行正常的类型转换
template <typename T>
void compare(const T&,const T&){}
void test5(){
	long l=100;
	//compare(l,1024);//错误，模板类型不匹配，一个是long，一个是int
	compare<long> (l,1024);//compare(long,long)
	compare<int> (l,1024);//compare(int,int)
}
//无法显式指定类型时，可以使用尾置返回类型
template <typename It>
auto fcn(It begin,It end)->decltype(*begin){//返回的时引用类型
	return *begin;
}
void test6(){
	int ia[]={1,2,3,4,5,6};
	vector<int> vi={2,3,4,5};
	cout<<fcn(begin(ia),end(ia))<<endl;//1
	cout<<fcn(vi.begin(),vi.end())<<endl;//2
}
//组合使用remove_reference、尾置返回以及decltype可以返回元素值的拷贝
template <typename It>
auto fcn_copy(It beg,It end)->typename remove_reference<decltype(*beg)>::type{
	return *beg;
}
void test7(){
	int ia[]={1,2,3,4,5,6};
	vector<int> vi={2,3,4,5};
	cout<<fcn_copy(begin(ia),end(ia))<<endl;//1
	cout<<fcn_copy(vi.begin(),vi.end())<<endl;//2
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	test7();
}