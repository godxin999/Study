/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/19 20:35:11
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <type_traits>

template <typename T>
std::enable_if_t<(sizeof(T)>4),T>//第二个参数为返回类型
foo1(){
	std::cout<<"foo1"<<std::endl;
	return T(123456789);
}

template <typename T>
std::enable_if_t<(sizeof(T)>4)>//如果不给出第二个参数则返回类型为void
foo2(){
	std::cout<<"foo2"<<std::endl;
}
void test1(){
	long long a=foo1<long long>();
	std::cout<<a<<std::endl;
	foo2<long long>();
}
//使用默认模板参数替代声明
template <typename T,typename=std::enable_if_t<(sizeof(T)>4)>>
void foo3(){
	std::cout<<"foo3"<<std::endl;
}
//使用别名模板使约束更明显
template <typename T>
using enableIfSizeGreaterThan4=std::enable_if_t<(sizeof(T)>4)>;

template <typename T,typename=enableIfSizeGreaterThan4<T>>
void foo4(){
	std::cout<<"foo4"<<std::endl;
}

void test2(){
	foo3<long long>();
	foo4<long long>();
}

int main(){
	//test1();
	test2();
	return 0;
}