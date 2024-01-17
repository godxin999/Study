/*************************************************************************
	> File Name: 8.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/14 21:05:40
 ************************************************************************/

#include <iostream>

//C++20位域的默认初始化
struct S{
	int a:8=11;
	int b:4{7};
};
//当位域的常量表达式是条件表达式时，使用括号保证优先级
int aa=16;
struct S1{
	int a:(true?8:aa)=42;
	int b:(true?4:aa){0};
};

int main(){
	S s;
	std::cout << s.a << std::endl;
	std::cout << s.b << std::endl;
	S1 s1;
	std::cout << s1.a << std::endl;
	std::cout << s1.b << std::endl;
	return 0;
}