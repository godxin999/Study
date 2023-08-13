/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/13 17:22:37
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include "..\2.1\stack.h"
//类模板的类型推导
void test1(){
	Stack<int> intStack;
	Stack<int> intStack2=intStack;
	Stack intStack3=intStack;//C++17后编译器可以自动推导出类模板类型
}
void test2(){
	//定义Stack(const T& elem):elems({elem}){}
	Stack s=0;//C++17后可被推导为Stack<int>
	//需要向编译器要求生成默认构造函数及其全部默认行为
	//使用{elem}初始化列表初始化elems
}
//推导指引
Stack(const char*)->Stack<std::string>;
void test3(){
	//不使用推导指引时，会被实例化为Stack<const char[7]>
	//Stack stringstack="bottom";//在使用推导指引后，这行代码是非法的
	Stack stringstack{"bottom"};
	std::cout<<stringstack<<std::endl;
	Stack stringstack2=stringstack;//模板参数推断的结果可以拷贝
	std::cout<<stringstack2<<std::endl;
}


int main(){
	test1();
	test2();
	test3();





}