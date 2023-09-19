/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/18 20:58:51
 ************************************************************************/

#include <iostream>
#include <string>
#include <bitset>
#include <cstring>
#include <vector>
#include "boolstring.hpp"
#include "stack.hpp"

void test1(){
	Stack<int> si;
	Stack<float> sf;
	si.push(1);
	si.push(2);
	sf.push(1.1);
	sf.push(2.2);
	si=sf;
	Stack<std::string> ss;
	ss.push("123");
	//sf=ss;无法将string类型转换为float类型
}

void test2(){
	Stack<int,std::vector<int>> vSi;
	vSi.push(42);
	vSi.push(7);
	std::cout<<vSi.top()<<std::endl;
	Stack<float> vSf;
	vSf.push(42.1);
	vSf.push(7.6);
	vSi=vSf;
}

void test3(){
	std::cout<<std::boolalpha;
	BoolString s1("hello");
	std::cout<<s1.get()<<std::endl;//hello
	std::cout<<s1.get<bool>()<<std::endl;//false
	BoolString s2("on");
	std::cout<<s2.get<bool>()<<std::endl;//true
}
//使用template防止<被理解为小于号，而不是模板参数的开始
template <unsigned long long N>
void printBitset(const std::bitset<N>& bs){
	//.template和->template、::template都是为了告诉编译器后面的<不是小于号，而是模板参数的开始
	std::cout<<bs.template to_string<char,std::char_traits<char>,std::allocator<char>>()<<std::endl;
}
void test4(){
	std::bitset<5> bs;
	bs.flip(4);
	printBitset(bs);//10000
}
//泛型lambda是一种成员模板的简化
//对于[](auto x,auto y){return x+y};编译器会默认为其构造下面这样一个类
class Lambda{
public:
	Lambda(){}
	template <typename T,typename T2>
	auto operator()(T x,T2 y)const{
		return x+y;
	}
};


int main(){
	//test1();
	//test2();
	//test3();
	test4();
	return 0;
}