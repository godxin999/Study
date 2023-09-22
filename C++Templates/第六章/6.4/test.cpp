/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/19 21:07:38
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <type_traits>
#include <utility>

template <typename T>
using enableIfString=std::enable_if_t<std::is_convertible_v<T,std::string>>;//std::is_convertible_v<>要求类型之间可以隐式转换
//可以使用std::is_constructible_v<>来判断类型是否可以构造


class Person{
private:
	std::string name;
public:
	template <typename T,typename=enableIfString<T>>
	explicit Person(T&& n):name(std::forward<T>(n)){
		std::cout<<"Person(T&& n)"<<std::endl;
	}
	Person(const Person& p):name(p.name){
		std::cout<<"Person(const Person& p)"<<std::endl;
	}
	Person(Person&& p):name(std::move(p.name)){
		std::cout<<"Person(Person&& p)"<<std::endl;
	}
};
void test1(){
	std::string s="name";
	Person p1(s);
	Person p2("name");
	Person p3(p1);
	Person p4(std::move(p1));
}
//禁用某些成员函数
template <typename T>
class C{
public:
	//删除const volatile& 类型的构造函数后，编译器不会再隐式声明一个接受const参数的构造函数
	//但是对于volatile类型，会发生报错
	C(const volatile C&)=delete;
	template <typename U,typename=std::enable_if_t<!std::is_integral_v<U>>>
	C(const C<U>&){
		std::cout<<"template ctor"<<std::endl;
	}
	C()=default;
};
void test2(){
	C<double> x;
	C<double> y{x};//调用模板拷贝构造函数
	C<int> xc;
	//C<int> yc{xc};//C<int>的拷贝构造函数被删除
}

int main(){
	//test1();
	test2();

	return 0;
}