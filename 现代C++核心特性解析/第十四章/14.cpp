/*************************************************************************
	> File Name: 14.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/15 17:57:06
 ************************************************************************/

#include <iostream>
//C++17支持列表初始化枚举类型
enum class Color: unsigned int{
	red,
	blue,
	green
};
void test1(){
	Color c = Color::red;
	Color c2{2};
}
//C++20支持使用using打开枚举类型
void test2(){
	Color c{0};
	using enum Color;
	switch(c){
		case red:
			std::cout << "red" << std::endl;
			break;
		case blue:
			std::cout << "blue" << std::endl;
			break;
		case green:
			std::cout << "green" << std::endl;
			break;
	}
}
int main(){
	//test1();
	test2();

	return 0;
}