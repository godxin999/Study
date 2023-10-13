/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/13 10:56:48
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//CPL.1 优先使用C++而不是C
//CPL.2 如果必须使用C，请使用C和C++的公共子集，并以C++的方式编译C代码

void test1(){
	std::cout<<sizeof('a')<<std::endl;//1
}

void test2(){
	char c='a';
	void* pv=&c;
	//int* pi=pv;//C++中会报错
	int* pi=static_cast<int*>(pv);
}

//CPL.3 如果必须使用C接口，那么在调用此类接口的代码里使用C++
//C++中，编译器会对函数进行名字重编，从而识别各种函数，我们可以使用extern "C"来阻止编译器对函数名字进行重编，从而达到在C中调用C++函数或在C++中调用C函数的目的
//使用在函数前
extern "C" void f(int){//和C++函数构成重载
	printf("C\n");
}
void f(double){
	std::cout<<"C++"<<std::endl;
}
void test3(){
	f(1);//C
	f(1.0);//C++
}
//使用在作用域前
extern "C" {
	void foo(int);
	void bar(int);
}
//使用包含保护宏来使用extern "C"
#ifdef __cplusplus
extern "C"{
#endif
//...
#ifdef __cplusplus
}
#endif


int main(){
	//test1();
	//test2();
	test3();
	return 0;
}