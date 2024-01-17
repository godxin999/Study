/*************************************************************************
	> File Name: 3.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/8 19:19:19
 ************************************************************************/

#include <iostream>

//auto和初始化列表的组合
void test1(){
	auto x1={1,2};//必须为相同类型的元素，x1的属性为std::initializer_list<int>
	auto x2{1};//只能使用一个值初始化，x2的属性为int
}
//C++14支持auto对返回类型推导
auto sum(int a,int b){
	return a+b;
}
//C++20支持auto对函数参数类型推导
void test2(auto x){
	std::cout<<sum(x,x)<<std::endl;
}
//C++14支持auto对lambda表达式的参数类型推导
void test3(){
	auto f=[](auto x,auto y){return x+y;};
	std::cout<<f(1,2)<<std::endl;
}
//C++14支持使用auto让lambda表达式返回引用
void test4(){
	auto f=[](auto& i)->auto&{
		return i;
	};
	int x=10;
	auto& x2=f(x);
	std::cout<<std::boolalpha<<(std::addressof(x)==std::addressof(x2))<<std::endl;
}
//C++17支持auto推导非类型模板参数
template <auto M>
void f(){
	std::cout<<M<<std::endl;
}
void test5(){
	f<5>();
	f<'a'>();
	f<true>();
	//f<2.0>();//error，模板参数不能为double类型
}


int main(){
	//test1();
	//test2(10);
	//test3();
	//test4();
	test5();
	return 0;
}