/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/24 10:32:02
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <type_traits>

template <typename T>
class C{
	static_assert(!std::is_same_v<std::remove_cv_t<T>,void>,"invalid instantiation of class C for void type");
public:
	template <typename V>
	void f(V&& v){
		if constexpr(std::is_reference_v<T>){//编译期if可以由std::enable_if、部分特例化和SFINAE替代

		}
		if constexpr(std::is_convertible_v<std::decay_t<V>,T>){//判断V是否可以隐式转换为T

		}
		if constexpr(std::has_virtual_destructor_v<T>){

		}
	}
};
//使用类型萃取的时候需要注意对应的规则
void test1(){
	std::remove_const_t<std::remove_reference_t<const int&>> a=1;//a为int类型
	std::remove_reference_t<std::remove_const_t<const int&>> b=1;//b为const int类型
	std::decay_t<const int&> c=1;//c为int类型
	std::remove_const_t<const int*const> d;//d为const int*类型，std::remove_const_t只会去除顶层const
}
void test2(){
	std::make_unsigned_t<int> a=1;
	//std::make_signed_t<const int&> b=1;//未定义行为
	std::add_rvalue_reference_t<const int> c=1;//const int &&c
	std::add_rvalue_reference_t<const int&> d=1;//const int &d，引用折叠导致类型为左值引用
}
void test3(){
	auto b=std::is_copy_assignable_v<int>;//检查一个int能否赋值给另一个(左值相关操作)
	auto b2=std::is_assignable_v<int,int>;//检查一个int能否赋值给另一个(右值相关操作)因为不能调用42=42所以为false
	std::cout<<std::boolalpha<<b<<" "<<b2<<std::endl;//true false
	auto b3=std::is_swappable_v<int>;//左值相关操作
	auto b4=std::is_swappable_v<int&>;//同上
	auto b5=std::is_swappable_with_v<int,int>;//右值相关操作，因为std::swap要求左值引用的参数，所以为false
	std::cout<<std::boolalpha<<b3<<" "<<b4<<" "<<b5<<std::endl;//true true false
}
//使用std::addressof()可以获取对象的地址，即使该对象重载了&操作符
template <typename T>
void f(T&& x){
	auto p=&x;//可能因为重载operator&而失效
	auto q=std::addressof(x);
}
//使用std::declval在不创建对象的情况下，推导对象类型或对象成员函数的返回值类型，因为std::declval返回右值引用类型，所以需要使用std::decay_t去除引用
template <typename T1,typename T2,typename RT=std::decay_t<decltype(true?std::declval<T1>():std::declval<T2>())>>
RT max(T1 a,T2 b){
	return b<a?a:b;
}

int main(){
	//test1();
	//test2();
	test3();
	return 0;
}