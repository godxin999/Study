/*************************************************************************
	> File Name: 10.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/14 21:51:42
 ************************************************************************/

#include <iostream>
#include <type_traits>
//使用=default保证类的平凡性质
class A{
	int i;
public:
	A(int n):i(n),j(n){}
	A(){}
	int j;
};
class B{
	int i;
public:
	B(int n):i(n),j(n){}
	B()=default;
	int j;
};
void test1(){
	std::cout<<std::boolalpha;
	std::cout<<std::is_trivial_v<A><<std::endl;
	std::cout<<std::is_trivial_v<B><<std::endl;
}
//=default可以添加到类外部，=delete必须在第一次声明时就添加
struct type{
	type();
	~type()=delete;
	int x;
};
type::type()=default;
//=delete还可以删除普通函数
int main(){
	test1();
	return 0;
}