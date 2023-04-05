/*************************************************************************
	> File Name: item8.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/5 23:25:36
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <mutex>
#include <memory>
using namespace std;
//优先考虑nullptr而不是NULL和0
void f(int){
	cout<<"f(int)"<<endl;
}
void f(bool){
	cout<<"f(bool)"<<endl;
}
void f(void*){
	cout<<"f(void*)"<<endl;
}
void test1(){
	f(0);
	f(NULL);//可能不会被编译，一般来说调用f(int)，绝对不会调用f(void*)
	f(nullptr);//nullptr可以转换为指向任何数据类型的指针
}
int main(){
	test1();
}