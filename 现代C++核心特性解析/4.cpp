/*************************************************************************
	> File Name: 4.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/12 11:46:04
 ************************************************************************/

#include <iostream>

//typied
//1.typeid返回的值是左值
//2.typeid返回的std::type_info的复制构造函数是delete的
//3.typeid(const T)==typeid(T)
void test1(){
	int a=0;
	double b=42.0;
	std::cout<<typeid(a).name()<<std::endl;//int
	std::cout<<typeid(a+b).name()<<std::endl;//double
	std::cout<<typeid(double).name()<<std::endl;//double
}
//decltype
//1.如果是没有括号的标识符表达式(除结构化绑定)或者没有括号的类成员访问，decltype返回的类型就是该标识符表达式或者类成员访问的类型
//2.如果是函数调用或者仿函数调用，decltype返回的类型就是函数的返回类型
//3.如果是左值，decltype返回的类型就是T&，其中T是左值的类型
//4.如果是将亡值，decltype返回的类型就是T&&，其中T是将亡值的类型
//5.其他情况，decltype返回的类型就是表达式结果类型

const int&& foo();
int i;
struct A{
	double x;
};
const A* a=new A{0};
int *j=&i;
int n[10];
struct B{
	int operator()(){return 0;}
}b;
void test2(){
	decltype(foo()) aa=10;//满足规则2，函数调用的返回类型为const int&&，所以aa为const int&&
	decltype(i) bb;//满足规则1
	decltype(a->x) c;//满足规则1
	decltype((a->x)) d=100;//因为带有括号，不满足规则1，满足规则3，表达式(a->x)为左值，所以d为double&
	decltype(static_cast<short>(i)) e;//满足规则5，表达式static_cast<short>(i)返回右值，所以e为short
	decltype(j) f;//满足规则1
	decltype(n) g;//满足规则1
	decltype(foo) h;//满足规则1
	decltype(b()) k;//满足规则2，仿函数调用的返回类型为int，所以k为int
	decltype(i=0) l=i;//满足规则3，表达式返回i为左值，所以l为int&
	decltype(0,i) m=i;//满足规则3，表达式返回i为左值，所以m为int&
	decltype(i,0) o;//满足规则5，表达式返回0为右值，所以o为int
	decltype(n[5]) p=i;//满足规则3，表达式n[5]为左值，所以p为int&
	decltype(*j) q=i;//满足规则3，表达式*j为左值，所以q为int&
	decltype(static_cast<int&&>(i)) r=10;//满足规则4，表达式static_cast<int&&>(i)返回将亡值，所以r为int&&
	decltype(i++) s;//满足规则5，表达式i++返回右值，所以s为int
	decltype(++i) t=i;//满足规则3，表达式++i返回左值，所以t为int&
	decltype("hello world") u="hello world";//满足规则3，"hello world"为常量数组的左值，所以u为const char(&)[12]
}
//cv限定符的推导
void test3(){
	const int i=0;
	decltype(i) a=i;//const int
	struct A{
		double x;
	};
	const A* aa=new A{0};
	//当decltype应用于未加括号的成员变量时，父对象表达式的cv限定符会被忽略
	decltype(aa->x) x=0;//double
	//当decltype应用于加了括号的成员变量时，父对象表达式的cv限定符会被保留
	decltype((aa->x)) y=x;//const double &
}
//decltype(auto)
//decltype(auto)的规则和decltype的规则一样，但是其不能结合指针、引用以及cv限定符
int&& f(){return 0;};
void test4(){
	auto x1a=i;//int
	decltype(auto) x1d=i;//int
	auto x2a=(i);//int
	decltype(auto) x2d=(i);//int&
	auto x3a=f();//int
	decltype(auto) x3d=f();//int&&
	auto x4a={1,2};//std::initializer_list<int>
	//decltype(auto) x4d={1,2};//{1,2}不是表达式，编译失败
	auto *x5a=&i;//int*
	//decltype(auto) *x5d=&i;//decltype(auto)不能和指针结合，编译失败
}
//C++17支持decltype(auto)推导非类型模板参数
template<decltype(auto) N>
void func(){
	std::cout<<N<<std::endl;
}
static const int x=11;
static int y=7;
void test5(){
	func<x>();//const int
	func<(x)>();//const int&
	//f<y>();//编译失败，因为y不是一个常量，所以不能作为非类型模板参数
	func<(y)>();//int&，对于静态对象，内存地址是固定的，所以可以通过编译
	func<true>();
}
int main(){
	//test1();
	test5();



	return 0;
}