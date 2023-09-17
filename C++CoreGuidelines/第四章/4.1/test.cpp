/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/17 13:37:30
 ************************************************************************/

#include <iostream>
#include <vector>
#include <cstring>

//F.1 将有意义的操作打包为精心命名的函数
//F.2 一个函数应该执行单一的操作
//F.3 使函数保持简短

void read_and_print(){//该函数没有保证单一职责
	int x;
	std::cin>>x;
	std::cout<<x<<std::endl;
}

int read(std::istream& is){
	int x;
	is>>x;
	//Error check
	return x;
}

void print(std::ostream& os,int x){
	os<<x<<std::endl;
}

void test1(){
	int x=read(std::cin);
	print(std::cout,x);
}

//F.4 如果函数有可能需要在编译器求值，把它声明为constexpr
//constexpr函数的好处为其在编译期运行，还可以用只能在运行期求值的参数来调用constexpr函数，constexpr函数是隐含内联的
//编译器求值的结果通常会被系统标记为只读，且编译器求值的constexpr函数为纯函数，其线程安全

constexpr auto gcd(int a,int b){
	while(b!=0){
		auto t=b;
		b=a%b;
		a=t;
	}
	return a;
}

void test2(){
	constexpr auto x=gcd(11,121);//直接从只读区域中获取
	int a=11;
	int b=121;
	int j=gcd(a,b);//调用函数获取
}

//F.6 如果还能书必定不抛出异常，则把其声明为noexcept
//即使函数可以抛出异常，noexcept有时也合理，因为此时表示程序无法对异常做出反应，系统处理异常的唯一办法就是调用std::terminate()
//但是不要随处添加noexcept声明，需要在考虑可能的异常能否被处理后再决定
//对于析构函数、swap函数、移动操作和默认构造函数永远不应该抛异常
//下面的函数会在内存耗尽时崩溃
std::vector<std::string> collect(std::istream& is)noexcept{
	std::vector<std::string> res;
	for(std::string s;is>>s;){
		res.push_back(s);
	}
	return res;
}

void test3(){
	auto v=collect(std::cin);
	for(auto& s:v){
		std::cout<<s<<std::endl;
	}
}

//F.8 优先使用纯函数
//纯函数为在给定相同参数时总返回相同结果的函数，纯函数的行为就像一个无限大的查找表
//纯函数的好处为其可以被孤立的测试、验证或重构，可以缓存其结果，可以被自动重排或在其他线程上执行
//square函数就是一个纯函数
template<typename T>
auto square(T t){
	return t*t;
}

int main(){
	//test1();
	//test2();
	test3();

	return 0;
}