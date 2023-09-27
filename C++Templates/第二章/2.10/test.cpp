/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/13 19:09:17
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//聚合类的模板化
template<typename T>
struct ValueWithComment{
	T value;
	std::string comment;
};
void test1(){
	ValueWithComment<int> vc;
	vc.value=0;
	vc.comment="hello";
}
//聚合类的推导指引
ValueWithComment(const char*,const char*)->ValueWithComment<std::string>;
void test2(){
	ValueWithComment vc{"hello","world"};
	//ValueWithComment vc("hello","world");//两种初始化方式均可
	cout<<vc.value<<endl;
	cout<<vc.comment<<endl;
}
int main(){
	test1();
	test2();





}

