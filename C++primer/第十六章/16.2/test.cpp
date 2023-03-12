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
#include <utility>
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
//函数指针和实参推断
/*
template <typename T>
void compare(const T&,const T&){}
*/
void func(void(*)(const string&,const string&)){}
void func(void(*)(const int&,const int&)){}
void test8(){
	//func(compare);//错误，无法确定使用哪个实例
	func(compare<int>);//正确，使用显式模板实参消除歧义
}
//类型推断、引用折叠
template <typename T>
void f1(T&){}//绑定左值
template <typename T>
void f2(const T&){}//绑定左值和const右值
template <typename T>
void f3(T&& val){//绑定非const左值
	T t=val;//传入右值会进行拷贝,传入左值会把t绑定到val
}
void test9(){
	int i=100;
	const int ci=20;
	f1(i);
	f1(ci);
	//f1(5);//错误，传递给一个&参数的实参必须是左值

	f2(i);
	f2(ci);
	f2(5);//一个const&参数可以绑定到一个右值，T为int
	
	//T& &,T& &&,T&& &均折叠为T&
	//T&& &&折叠为T&&
	f3(i);//T为int&，int& &&折叠为int&
	f2(ci);//T为constint&，同理进行折叠
	f3(42);//实参为一个int类型的右值
}
//保持类型信息的函数
template <typename F,typename T1,typename T2>
void flip(F f,T1 &&t1,T2 &&t2){
	f(forward<T2>(t2),forward<T1>(t1));//
}
void f(int v1, int &v2){
	cout<<v1<<" "<<++v2<<endl;
}
void g(int &&i,int &j){
	cout<<i<<" "<<j<<endl;
}
void test10(){
	int i=0;
	flip(g,i,42);
	flip(f,i,42);
	flip(f,i,i);
	//flip(g,i,i);//错误，右值引用不能绑定到左值
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7();
	//test8();
	//test9();
	test10();
}