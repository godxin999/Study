/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/11 20:23:41
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include "stack.h"

int main(){
	Stack<int> is;
	Stack<std::string> ss;
	is.push(7);
	std::cout<<is.top()<<std::endl;
	ss.push("hello");
	std::cout<<ss.top()<<std::endl;
	ss.pop();

}