/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/20 11:56:08
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <functional>
#include <cstring>

void printString(const std::string& s){
	std::cout<<s<<std::endl;
}
template <typename T>
void print(T arg){
	printString(arg);
}

template <typename T>
void printT(T arg){
	std::cout << arg << std::endl;
}

void test1(){
	std::string s="hello";
	print(s);
	print(std::cref(s));//将一个std::reference_wrapper<const std::string>对象按值传递给参数arg，这样std::reference_wrapper<const std::string>对象就会被传入函数模板并被转换为原始参数类型std::string
	//printT(std::cref(s));//如果尝试直接输出传递进来的类型为T的对象，则会报错，因为std::reference_wrapper<const std::string>没有重载<<
}
//下列代码同样报错，因为std::reference_wrapper<const std::string>无法和const char *或者std::string类型进行比较
template <typename T1,typename T2>
bool isLess(T1 arg1,T2 arg2){
	return arg1<arg2;
}
void test2(){
	std::string s="hello";
	//auto b=isLess(std::cref(s),"world");
	//auto b2=isLess(std::cref(s),std::string("world"));
}

int main(){
	//test1();
	test2();




}