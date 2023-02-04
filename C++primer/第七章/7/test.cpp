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
class Ac {
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
	//X2(int val) :j(val), i(j) {}//i先出现且i使用j进行初始化，这时j没有被初始化，所以i没有被初始化
};
//类必需要包括一个默认构造函数
/*
class NoDefault {
public:
	NoDefault(const std::string&);//不再产生默认构造函数
};
struct A {
	NoDefault my_mem;//错误，无法为A合成构造函数
};
//A a;
struct B {
	B() {}//错误，b_mem无初始值
	NoDefault b_mem;
};
*/
//测试字面值常量类
class Debug {
public:
	constexpr Debug(bool b = true) :hw(b), io(b), other(b) {}
	constexpr Debug(bool h, bool i, bool o) : hw(h), io(i), other(o) {}
	constexpr bool any()const { return hw || io || other; }//必须加上只读的const，常量只能调用对应常量的函数
	void set_io(bool b) { io = b; }
	void set_hw(bool b) { hw = b; }
	void set_other(bool b) { other = b; }
private:
	bool hw;
	bool io;
	bool other;
};
//静态成员可以是不完全类型
class Bar {
public:
private:
	static Bar mem1;//静态成员可以是不完全类型
	Bar* mem2;//指针成员可以是不完全类型
	//Bar mem3;//报错，数据成员必须是完全类型
};
//静态成员可用作默认实参
class Screen {
public:
	Screen& clear(char = bkground);//静态成员可用作默认实参
private:
	static const char bkground;
};
//静态成员的使用
class Account {
public:
	static double rate() { return iRate; }
	static void rate(double);
private:
	double amount;
	static constexpr int period = 100;//类内初始化，必须为字面值类型的constexpr，在外部如果作为参数需要重新定义一下
	static double iRate;//在类外初始化
	static double initRate();
};
//double Account::iRate = 100;//初始化
void Account::rate(double newrate) { iRate = newrate; }//静态成员的定义
double Account::initRate() { iRate = 100; return iRate; }
double Account::iRate = initRate();//域作用符之后就代表在类内，可以直接调用私有函数
constexpr int Account::period;//不带初始值的静态成员的定义，初始值在类内提供

int main(){
	//字面值常量类测试
	/*
	constexpr Debug io_sub(false, true, false);
	if (io_sub.any()) {
		cerr << "print approprite error messages" << endl;
	}
	constexpr Debug prod(false);
	if (prod.any()) {
		cerr << "print an error message" << endl;
	}
	*/
	double r;
	r = Account::rate();//不能在全局调用(函数体外面调用函数）
	Account ac1;
	Account* ac2 = &ac1;
	r = ac1.rate();
	r = ac2->rate();
}

