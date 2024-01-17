/*************************************************************************
	> File Name: 5.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/12 13:20:16
 ************************************************************************/

#include <iostream>

//返回复杂类型时使用后置返回类型
int bar_impl(int x){
	return x;
}
auto foo()->int(*)(int){
	return bar_impl;
}
auto foo2()->decltype(bar_impl)*{
	return bar_impl;
}
//推导函数模板返回类型时使用后置返回类型
template<typename T,typename U>
auto add(T x,U y)->decltype(x+y){
	return x+y;
}

int main(){
	auto x=add(1,2);
	std::cout<<x<<std::endl;
	auto f1=foo();
	auto f2=foo2();
	std::cout<<f1(1)<<std::endl;
	std::cout<<f2(2)<<std::endl;


	return 0;
}