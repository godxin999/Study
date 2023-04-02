/*************************************************************************
	> File Name: item6.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/2 22:34:13
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
//auto推导使用代理类的对象会产生意外的错误
vector<bool> features(int){return vector<bool> {0,1,1,0,1,1,0,1,0};}
void test1(){
	bool v1=features(100)[5];
	auto v2=features(100)[5];//v2的类型为std::vector<bool>::reference，其可能包含指向features函数返回的临时变量vector<bool>的指针，此时临时变量已经被销毁，则该指针为空悬指针
}
//使用惯用的显示类型初始化
void test2(){
	auto v=static_cast<bool> (features(100)[5]);
}

int main(){
	//test1();
	test2();
}