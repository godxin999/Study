/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/2 22:53:53
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
//auto的使用
void test1(){
	const int i=10;
	auto v=i;//v的类型为int
	auto &rv=i;//rv的类型为const int&
	const int* const p=&i;
	auto &pci1=p;//pci1的类型为const int* const&，当auto声明为指针或引用时，只忽略表达式的引用属性
	auto pci2=p;//pci2的类型为const int*，当auto不声明指针或引用时，忽略表达式的引用属性和顶层cv限定符
}
//auto使用受限的情况
//void f(auto i=1){}//auto不能用于函数的参数
struct foo{
	//auto i=0;//auto不能用于非静态成员变量
	static const auto j=0;//只能用于静态常量
};
template <typename T>
struct Bar{};
void test2(){
	int arr[10]={0};
	auto aa=arr;
	//auto rr[10]=arr;//错误，auto不能声明数组
	Bar<int> bar;
	//Bar<auto> bb=bar;//错误，auto不能推导出模板参数
}
//使用auto
class A{
public:
	static int get(){
		return 0;
	}
};
class B{
public:
	static const char* get(){
		return "0";
	}
};
template<class C>
void func(){
	auto val=C::get();//对所有具有静态方法get的类型C，在获得get获得结果后做统一的处理
}
void test3(){
	func<A> ();
	func<B> ();
}
//decltype的使用
class Foo{
public:
	static const int Number=0;
	int x;
};
int& func_int_r();
int&& func_int_rr();
int func_int();
const int& func_cint_r();
const int&& func_cint_rr();
const int func_cint();
const Foo func_cfoo();
void test4(){
	int x=0;
	decltype(func_int_r()) a1=x;//int&
	decltype(func_int_rr()) b1=0;//int&&
	decltype(func_int()) c1=0;//int
	
	decltype(func_cint_r()) a2=x;//const int&
	decltype(func_cint_rr()) b2=0;//const int&&
	decltype(func_cint()) c2=0;//int，因为函数返回的是纯右值，所以会忽略cv限定符

	decltype(func_cfoo()) ff=Foo();//const Foo

	struct F{int x;};
	const F f=F();

	decltype(f.x) a=0;//int
	decltype((f.x)) b=a;//const int&，f的类型是const F，所以f.x的类型为const int，又因为括号表达式为左值，所以最终的类型为const int&

	int n=0,m=0;
	decltype(n+m) c=0;//int
	decltype(n+=m) d=c;//n+=m返回一个左值，所以d的结果为int&
}
//使用decltype
template <class Container>
class Foo2{
	decltype(Container().begin()) it_;//不用对const类型的迭代器类型进行特例化
public:
	void func(Container& container){
		it_=container.begin();
	}
};
void test5(){
	vector<int> v;
	Foo2 <vector<int>> f;
	f.func(v);
	decltype(v)::value_type i=0;//抽取表达式类型
}
//尾置返回类型
template <typename R,typename T,typename U>
R add(T t,U u){
	return t+u;
}
void test6(){
	int a=1;
	float b=2.0;
	auto c=add<decltype(a+b)>(a,b);
}
/*
template <typename R,typename T,typename U>
decltype(t+u) add2(T t,U u){//错误，t、u未定义
	return t+u;
}
*/
template <typename R,typename T,typename U>
decltype(T()+U()) add3(T t,U u){
	return t+u;
}
template <typename R,typename T,typename U>
decltype((*(T*)0)+(*(U*)0)) add4(T t,U u){//考虑T和U可能是没有无参构造函数的类
	return t+u;
}
template <typename R,typename T,typename U>
auto add5(T t,U u)->decltype(u+t){//使用尾置返回类型
	return t+u;
}

int &f1(int &i);
float foo(float& f);
template <typename T>
auto func(T& val)->decltype(foo(val)){//根据推导确定func的返回类型
	return foo(val);
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	test6();
}

