/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/1/31 19:37:46
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//友元声明影响访问权限，不是真正的声明。
struct X{
	friend void f(){}//友元函数可以在类内定义
	//X() { f(); }//错误，f未声明
	void g();
	void h();
};
//void X::g() { return f(); }//错误，f未声明
void f();
void X::h() { return f(); }//正确，f已经声明
//如果外层中定义了一个名字，这个名字是一种类型，那么在类中就不能重新定义这个名字
//一些编译器不会报错
typedef double Money;
class Account {
public:
	Money balance() { return bal; }
private:
	typedef double Money;//不能重新定义Money
	Money bal;
};
//const成员，引用，未提供默认构造函数的类类型需要初始值列表来提供初值
class ConstRef {
public:
	ConstRef(int ii);
private:
	int i;
	const int ci;
	int& ri;
};
/*
ConstRef::ConstRef(int ii){
	i = ii;
	ci = ii;//错误，不能给const赋值
	ri = i;//错误，ri没有被初始化
}
*/
ConstRef::ConstRef(int ii) :i(ii), ci(ii), ri(i) {}//正确，显式初始化引用和const成员
//成员按照在类中出现的顺序进行初始化
class X2 {
	int i;
	int j;
public:
	X2(int val) :j(val), i(j) {}//i先出现且i使用j进行初始化，这时j没有被初始化，所以i没有被初始化
};




int main(){

}

