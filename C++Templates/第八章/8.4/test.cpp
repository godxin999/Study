/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/21 16:23:19
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>

template <typename  T,unsigned N>
std::size_t len(T(&)[N]){
	return N;
}

template <typename T>
typename T::size_type len(const T& t){
	return t.size();
}
//提供对所有类型的应急选项，但是对于test4()，其还是会匹配到第二个函数模板，因为第二个函数模板是最佳匹配项
std::size_t len(...){
	return 0;
}

void test1(){
	//第二个函数模板也可以使用int[10]和const char[4]来替换类型参数T，但是这种替换在处理返回类型T::size_type会导致错误，所以第二个模板被忽略
	int a[10];
	std::cout<<len(a)<<std::endl;
	std::cout<<len("tmp")<<std::endl;
}
void test2(){
	std::vector<int> v;
	std::cout<<len(v)<<std::endl;//只有第二个函数模板被匹配
}
void test3(){
	int *p;
	std::cout<<len(p)<<std::endl;//两个函数模板均不会被匹配，但是匹配到应急选项
}
void test4(){
	std::allocator<int> alloc;
	//std::cout<<len(alloc);//编译器匹配到第二个模板，但是std::allocator没有size()成员函数，此时第二个函数模板不会被忽略
}
//通过decltype进行SFINAE
//使用尾置返回类型来指定返回类型
//通过decltype和逗号运算符定义返回类型
//将所有需要成立的表达式放在逗号运算符的前面(为了防止运算符被重载，需要将这些表达式的类型转换为void)
//在逗号运算符的末尾定义一个类型为返回类型的对象
template <typename T>
auto len2(const T& t)->decltype((void)(t.size()),T::size_type()){//decltype的操作数不会被计算
	return t.size();
}

int main(){
	//test1();
	//test2();
	test3();
	//test4();

	return 0;
}