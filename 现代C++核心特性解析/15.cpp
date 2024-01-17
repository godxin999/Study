/*************************************************************************
	> File Name: 15.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/16 12:51:42
 ************************************************************************/

#include <iostream>
#include <type_traits>
#include <string>
//C++17扩展聚合类型
//1.没有用户提供的构造函数
//2.没有私有和受保护的非静态数据成员
//3.没有虚函数
//如果类存在继承关系，需要满足以下额外的条件
//4.必须是公开的基类，不能是私有或受保护的基类
//5.必须是非虚继承
class Mystring:public std::string{};
void test1(){
	std::cout << std::boolalpha;
	std::cout << std::is_aggregate_v<Mystring> << std::endl;//true
}
//聚合类型初始化(聚合初始化)
//如果派生类存在多个基类，其初始化的顺序要与继承的顺序相同
class Mystring2:public std::string{
public:
	int index=0;
};
std::ostream& operator<<(std::ostream& os,const Mystring2& s){
	os << s.index << " " << s.c_str();
	return os;
}
void test2(){
	std::cout << std::boolalpha;
	std::cout << std::is_aggregate_v<Mystring2> << std::endl;//true
	Mystring2 s{"hello",100};//聚合类型使用大括号直接初始化
	std::cout << s << std::endl;//100 hello
}
//C++20支持使用小括号初始化聚合类型
struct X{
	int i;
	float f;
};
void test3(){
	X x{1,2.0f};
	X x2(1,2.0f);
}
//C++20禁止聚合类型使用用户声明的构造函数
struct A{
	A()=default;
};
struct B{
	B()=delete;
};
void test4(){
	std::cout<<std::boolalpha;
	std::cout<<std::is_aggregate_v<A> << std::endl;
	std::cout<<std::is_aggregate_v<B> << std::endl;
	//false in C++20, true in C++17
}
int main(){
	//test1();
	//test2();
	//test3();
	test4();
	return 0;
}