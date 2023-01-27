/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/1/27 14:22:24
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//拥有顶层const的形参无法和没有顶层const的形参区分开
/*
void f(int){
	cout<<"int"<<endl;
}
void f(const int){
	cout<<"const int"<<endl;
}
//error: redefinition of 'f'
void f2(int*){
	cout<<"int"<<endl;
}
void f2(int* const){
	cout<<"const int"<<endl;
}
//error: redefinition of 'f2'
*/
//如果形参为某种类型的指针或者引用，则可以使用const区分其指向的是常量对象还是非常量对象，这里的const为底层，可以实现函数重载
void f3(int&){//作用于int的引用
	cout<<"int&"<<endl;
}
void f3(const int&){//作用于常量引用
	cout<<"const int&"<<endl;
}
void f4(int*){//作用于int的指针
	cout<<"int*"<<endl;
}
void f4(const int*){//作用于指向常量的指针
	cout<<"const int*"<<endl;
}
void test1(){
	int a=1;
	const int b=2;
	f3(a);//int&
	f3(b);//const int&
	f4(&a);//int*
	f4(&b);//const int*
}

int main(){
	test1();




}

