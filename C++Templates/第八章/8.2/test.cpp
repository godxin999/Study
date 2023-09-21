/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/21 15:42:09
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//C++14后判断是否为质数的函数
constexpr bool isPrime(unsigned p){
	for(unsigned d=2;d*2<p;++d){
		if(p%d==0){
			return false;
		}
	}
	return p>1;
}
bool fiftySevenIsPrime(){
	return isPrime(57);//编译器决定是否在编译期计算
}
const bool b=isPrime(43);//在全局作用域或者namespace作用域中，会在编译期进行计算

int main(){
	std::cout<<b<<std::endl;
	constexpr bool b1=isPrime(43);
	std::cout<<b1<<std::endl;
	const bool b2=isPrime(2001);
	std::cout<<b2<<std::endl;
	const bool b3=fiftySevenIsPrime();//在运行时计算
	std::cout<<b3<<std::endl;


	return 0;
}