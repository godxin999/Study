/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/20 19:55:16
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//确保模板函数采用按值返回的方式
template <typename T>
T ret1(T&& p){//T可能被推导为引用类型
	return T{};
}
template <typename T>
T ret2 (T p){//如果T是引用类型，那么返回值将会是引用类型
	return T{};
}
//使用类型萃取std::remove_reference_t<T>将T转换为非引用类型
template <typename T>
std::remove_reference_t<T> ret3(T p){
	return std::remove_reference_t<T>{};
}
//将返回类型声明为auto，让编译器来推到类型
template <typename T>
auto ret4(T p){
	return T{};
}

int main(){
	int a=100;
	int &b=a;
	//auto r1=ret1(a);
	//auto r2=ret2<int&>(a);
	auto r3=ret3<int&>(a);
	auto r4=ret4(b);

	return 0;
}