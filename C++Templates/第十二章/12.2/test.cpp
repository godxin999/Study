/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/27 14:55:58
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//模板参数分为：类型参数、非类型模板参数、模板模板参数
template <typename T,T root,template<T> class Buf>
class Structure;

//非类型参数表示一个可以在编译期或链接期确定的常量值，其参数类型必须是以下之一：
//整形或枚举类型
//(成员)指针类型
//左值引用类型
//std::nullptr_t
//包含auto或decltype(auto)的类型

//函数和数组类型可以被指定，但是它们会通过退化隐式地调整为相应的指针类型
template <int buf[5]>
class Lexer;
template <int* buf>
class Lexer;

template <int fun()>
struct FuncWarp;
template <int(*)()>
struct FuncWarp;

//非类型模板参数不可以有static、mutable指示符，但是可以有cv限定符，但是如果cv限定符为顶层，那么其会被忽略
template<const int l>
class Buf;
template<int l>//和上面的声明一样
class Buf;

//非引用类型的非类型参数始终为纯右值，它们的地址无法被窃取，也无法被赋值，左值引用类型的参数可以像左值一样使用
template <int& c>
struct LocalIncrement{
	LocalIncrement(){
		c=c+1;
	}
	~LocalIncrement(){
		c=c-1;
	}
};

//模板模板参数是类或别名模板的占位符，它们的声明和类模板很像，但是不能使用struct或union
//C++17开始支持使用typename来替代class
template<template<typename X>class C>
void f(C<int>* p){

}
//模板模板参数的参数可以有默认模板参数
class MyAllocator{};
template<template<typename T,typename A=MyAllocator>class Container>
class Adaptation{
	Container<int> storage;
};
//T和A都是模板模板参数的模板参数名称，这些名称仅可以在该模板模板参数的其他参数声明中使用

//模板参数包
//类型参数包
template <typename... Types>
class Tuple;
using IntTuple=Tuple<int>;
using IntCharTuple=Tuple<int,char>;
using IntTriple=Tuple<int,int,int>;
using EmptyTuple=Tuple<>;

//非类型模板参数包
template<typename T,unsigned... Dimensions>
class Array;
using matrix=Array<double,3,3>;

//模板模板参数包
template<typename T,template<typename,typename>class ... Containers>
void testContainers(){

}
//主模板中的类模板、变量模板和别名模板至多只可以有一个模板参数包，且模板参数包必须作为最后一个模板参数，函数蒙语版可以有多个模板参数包，只要模板参数包后面的每个参数都具有默认值或可以推导
template<typename... Types,typename T>
void runTests(T value){

}
template<unsigned... D1,unsigned... D2>
auto compose(Array<double,D1...> &a,Array<double,D2...> &b){
	
}
//类和变量模板的偏特化声明则可以有多个参数包
template<typename...> struct TypeList;
template<typename X,typename Y>struct Zip;
template<typename... Xs,typename... Ys>
struct Zip<TypeList<Xs...>,TypeList<Ys...>>{

};

//嵌套模板可以实现类型参数包的展开
template<typename... Types>
struct ArgList{
	template<Types... vals>
	struct Vals{};
};
ArgList<int,char,char>::Vals<1,'a','b'> v1;

//默认模板实参
template<typename T1,typename T2,typename T3,typename T4=char>
class Q;
template<typename T1,typename T2,typename T3=char,typename T4>
class Q;
/*
template<typename T1=char,typename T2,typename T3,typename T4>//错误，因为T2没有默认模板实参
class Q;
*/
//函数模板的默认模板实参不受上述约束
template<typename R=void,typename T>
R* addressOf(T& value);
//默认模板实参不允许重复声明
template<typename T=void>
class v;
/*
template<typename T=void>
class v;
*/
//在以下情景中不允许使用默认模板实参：偏特化、参数包、类模板成员类外定义、友元类模板声明、友元函数模板声明(定义除外)



int main(){





	return 0;
}