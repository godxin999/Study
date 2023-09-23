/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/23 14:55:13
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//一个是模板的类被称为类模板(class template)，也就是说它是一组类的参数化表达
//显式实例化(explicit specialization)/特例化
template <typename T1,typename T2>
class MyClass{};
template <>
class MyClass<int,double>{};
//部分特例化：特例化后仍有模板参数
template <typename T>
class MyClass<T,T>{};
//不完整类型：
//一个声明但没有定义的class
//一个没有指定边界的数组
//一个存储非完整类型的数组
//void类型
//一个底层类型未定义或者枚举值未定义的枚举类型
//任何一个被const或volatile修饰的以上某种类型
class C;
const C* pc;
extern C elems[10];
extern int arr[];
//唯一定义法则(ODR)
//常规(非模板)非inline函数和成员函数，以及非inline的全局变量和静态数据成员，在整个程序中只能定义一次
//class类型(包含struct和union)，模板(包含部分特例化，但不能是全特化)以及inline函数和变量，在一个编译单元中只能被定义一次，且在不同编译单元间的定义应该相同

template <typename T>//模板参数(template parameter)
class foo{};

int main(){
	foo<int> f;//int为模板实参(template argument)




	return 0;
}