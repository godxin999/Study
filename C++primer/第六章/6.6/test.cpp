/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/1/28 10:55:43
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//函数匹配
void f(){
	cout<<"none"<<endl;
}
void f(int){
	cout<<"int"<<endl;
}
void f(int,int){
	cout<<"int,int"<<endl;
}
void f(double,double=3.14){
	cout<<"double,double"<<endl;
}
void test1(){
	f(5.6);//double,double
}
//42和f(int,int)匹配更优，5.6和f(double,double)匹配更优，无法选择最优就报错
void test2(){
	//f(42,5.6);//error: call to 'f' is ambiguous
}
void f2(int){
	cout<<"int"<<endl;
}
void f2(short){//只有当实参为short的时候才会调用
	cout<<"short"<<endl;
}
//小整数类型会提升到int类型
void test3(){
	f2('a');//int
	f2(static_cast<short>(2));//short
}
void f3(long){
	cout<<"long"<<endl;
}
void f3(float){
	cout<<"float"<<endl;
}
//算数转换的级别均相同，double既可以转换为float也可以转换为long，故有二义性
void test4(){
	//f3(3.14);//error: call to 'f3' is ambiguous
}

int main(){
	//test1();
	//test2();
	//test3();
	//test4();


}

