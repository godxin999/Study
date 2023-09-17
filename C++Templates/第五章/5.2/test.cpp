/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/17 10:39:28
 ************************************************************************/

#include <iostream>

//显式调用默认构造函数将内置类型初始化为0
//以下两种方式均可，在C++11之前需要使用小括号进行初始化
template <typename T>
class MyClass{
private:
	T x{};
public:
	MyClass()=default;
};

template <typename T>
class MyClass2{
private:
	T x;
public:
	MyClass2():x{}{}
};
//函数模板默认参数的初始化方式如下

template <typename T>
void foo(T t=T{}){

}



int main(){





	return 0;
}