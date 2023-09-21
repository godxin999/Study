/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/21 17:23:21
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <limits>

template <typename T, typename... Args>
void print(const T& firstArg,const Args&... args){
	std::cout<<firstArg<<std::endl;
	if constexpr(sizeof...(args)>0){//如果只传递一个参数，那么sizeof...(args)的值为0，if语句中的代码不会被实例化
		print(args...);
	}
}

template <typename T>
void foo(T t){
	if constexpr(std::is_integral_v<T>){
		if(t>0){
			foo(t-1);
		}
	}
	else{
		//所谓的不会被实例化指的是指挥及逆行第一阶段编译，此时只会做语法检查和模板参数无关的名称检查
		/*undeclared(t);
		discarded(i.e. T is not integral);
		undeclared();
		static_assert(false,"no integral");
		static_assert(!std::is_integral_v<T>,"no integral");
		*/
	}
}

int main(){
	//if constexpr可以用于类型的函数，它需要的知识一个可以返回布尔值的编译器表达式
	if constexpr(std::numeric_limits<char>::is_signed){
		foo(42);
	}
	else{
		//...
	}
	return 0;
}