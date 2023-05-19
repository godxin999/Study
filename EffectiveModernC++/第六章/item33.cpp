/*************************************************************************
	> File Name: item33.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/19 9:03:00
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//lambda表达式实现完美转发
void otherfunc(){}
auto f=[](auto&&... param){
	return otherfunc(std::forward<decltype(param)>(param)...);
};

int main(){

}