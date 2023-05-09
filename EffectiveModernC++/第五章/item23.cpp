/*************************************************************************
	> File Name: item23.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/8 17:17:24
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <type_traits>
//C++11的move实现
template <typename T>
typename std::remove_reference<T>::type&& move(T&& param){//remove_reference确保&&被正确的应用到一个不是引用的类型上
	using ReturnType = typename std::remove_reference<T>::type&&;
	return static_cast<ReturnType>(param);//返回一个右值引用
}
//C++14的move实现
template <typename T>
decltype(auto) move(T&& param){
	using ReturnType=std::remove_reference_t<T>&&;
	return static_cast<ReturnType>(param);
}
//std::forward
class widget{};
void process(const widget&){
	std::cout<<"const widget&"<<std::endl;
}
void process(widget&&){
	std::cout<<"widget&&"<<std::endl;
}
template <typename T>
void func(T&& param){
	process(std::forward<T>(param));//将右值初始化的参数转换为右值，保留原有的右值或左值属性(转发)
}
void test1(){
	widget w;
	func(w);//const widget&
	func(std::move(w));//widget&&
}





int main(){
	test1();





}