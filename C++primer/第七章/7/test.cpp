/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/1/31 19:37:46
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//友元声明影响访问权限，不是真正的声明。
struct X{
	friend void f(){}//友元函数可以在类内定义
	//X() { f(); }//错误，f未声明
	void g();
	void h();
};
//void X::g() { return f(); }//错误，f未声明
void f();
void X::h() { return f(); }//正确，f已经声明

int main(){

}

