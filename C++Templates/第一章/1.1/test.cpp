/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/7/4 21:38:40
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <complex>
using namespace std;
//模板两阶段编译检查
complex<float> c1,c2;
void test1(){
	//max(c1,c2);//编译期error
	//模板分两步编译
	//1.模板定义阶段，检查包括
	//1)语法检查
	//2)未定义类型名、函数名
	//3)未使用模板参数的static_assertions
	//2.模板实例化阶段，对所有代码进行检查，尤其是依赖类型参数的部分
}
template <typename T>
void foo(T t){
	undeclared();//undeclared未定义，第一阶段报错
	undeclared(t);//undeclared(t)未定义，第二阶段报错，和类型参数有关
	static_assert(sizeof(int)>10,"int too small");
	static_assert(sizeof(T)>10,"T too small");//在第二阶段报错，和模板参数有关
}
int main(){
	test1();





}