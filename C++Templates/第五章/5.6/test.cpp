/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/18 22:37:01
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <array>

//变量模板(since C++14)
template <typename T=long double>
constexpr T pi{3.1415936535897932385};
template <typename T> T val{};
void test1(){
	val<long> =42;
	std::cout<<val<long><<std::endl;
	std::cout<<pi<><<std::endl;
	std::cout<<pi<float><<std::endl;
	//std::cout<<pi<<std::endl;//非法，无论怎样都要使用尖括号<>
}
//使用非类型参数对变量模板进行参数化，也可以将非类型参数用于参数器的初始化
template <int N>
std::array<int,N> arr{};
template <auto N>
constexpr decltype(N) dval=N;
void test2(){
	std::cout<<dval<'c'><<std::endl;//c
	//不同编译单元间初始化或遍历arr时，使用的都是同一个全局作用域里的arr
	arr<10>[0]=42;
	for(std::size_t i=0;i<arr<10>.size();++i){
		std::cout<<arr<10>[i]<<std::endl;
	}
}
//用于数据成员的变量模板
template <typename T>
class MyClass{
public:
	static constexpr int max=100;
};
template <typename T>
int myMax=MyClass<T>::max;
void test3(){
	auto i=myMax<std::string>;//用于替代MyClass<std::string>::max
	std::cout<<i<<std::endl;
	//从C++17开始，标准库使用变量模板为其用来产生一个值(布尔型)的类型萃取定义了简化方式
	//如使用std::is_const_v<T>代替std::is_const<T>::value
	//标准库的定义如下：
	//namespace std{
	//	template <typename T>
	//		constexpr bool is_const_v=std::is_const<T>::value;
	//}
}

int main(){
	//test1();
	//test2();
	test3();
	return 0;
}