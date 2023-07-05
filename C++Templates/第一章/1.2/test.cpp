/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/7/5 22:53:36
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
//类型推断中的类型转换
template<typename T>
T max(T a,T b){
	return a > b ? a : b;
}
void test1(){
	//max(4,7.2);//error
	//解决方案一(推荐)
	std::cout<<max(static_cast<double>(4),7.2)<<std::endl;
	//解决方案二
	std::cout<<max<double>(4,7.2)<<std::endl;
}
//对默认参数的类型推断
template <typename T>
void f(T=" "){

}
void test2(){
	f(1);
	//f();//error
}
//给模板类型参数也加上默认参数
template <typename T=std::string>
void f2(T=" "){

}
void test3(){
	f2(1);
	f2();
}

int main(){
	//test1();
	//test2();
	test3();



}

