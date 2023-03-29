/*************************************************************************
	> File Name: item2.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/29 22:48:22
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <type_traits>
#include <initializer_list>
#include <vector>
using namespace std;

template <typename T>
void func_for_x(T param){}
template <typename T>
void func_for_cx(const T param){}
template <typename T>
void func_for_rx(const T &param){}
//auto的推导大部分情况下和模板推导相同
void test1(){
	//编译器的行为类似认为每个声明均有一个模板，并采用合适的初始化表达式进行处理
	auto x=27;//func_for_x(27)
	const auto cx=x;//func_for_cx(x)
	const auto &rx=cx;//func_for_rx(x)
}
//数组和函数的模板推导同样适用于auto
void someFunc(int,double){}
void test2(){
	const char name[]="Zhang Xin";
	auto arr1=name;//arr1的类型为const char*
	auto &arr2=name;//arr2的类型为const char(&)[10]

	auto func1=someFunc;//func1的类型为void(*)(int,double)
	auto &fun2=someFunc;//func2的类型为void(&)(int,double)

	cout<<boolalpha<<is_same<decltype(func1),void(*)(int,double)>::value<<endl;
}
//auto和模板推导的不同
void test3(){
	auto x=27;//int
	auto x2(27);//int
	auto x3={27};//x3的类型被推导为std::initializer_list<int>，值为{27}
	auto x4{27};//同上

	cout<<boolalpha<<is_same<decltype(x3),std::initializer_list<int>>::value<<endl;
}
//初始化列表中的值不能为多种类型
void test4(){
	//auto x={1,2,3.0};//错误，首先通过auto推导出std::initializer_list，然后对std::initializer_list进行模板推导，因为花括号中的值不是同一种类型，所以推导出现错误
}
//auto可以推导出初始值列表但是模板推导不能
template <typename T>
void f(T param){}
template <typename T>
void f2(std::initializer_list<T> initList){}
void test5(){
	auto x={11,23,9};
	//f({11,23,9});//错误，类型使std::initializer_list<int>，模板无法推导出T
	f2({11,23,9});//正确，指定类型为std::initializer_list<T>，模板能推导出T，这里为int
}
//auto在函数返回值和lambda函数中的应用
/*
auto func(){
	return {1,2,3};//错误，推导错误，虽然函数返回值是auto，但是实际上是模板推导在起作用，模板推导不能推导出初始值列表
}
*/
void test6(){
	vector<int> v;
	auto resetv=[&v](const auto &newValue){v=newValue;};
	//resetv({1,2,3});//错误，模板推导不能推导出初始值列表
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	test6();
}

