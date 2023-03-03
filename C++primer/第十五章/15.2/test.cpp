/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/3 16:39:01
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//继承和静态成员
class Base{
public:
	static void statmem(){
		cout<<"statmem"<<endl;
	}
};
class Derived:public Base{
public:
	void f(const Derived&);
};
void Derived::f(const Derived &derived_obj){
	Base::statmem();//正确，Base定义了statmem
	Derived::statmem();//正确，Derived继承了statmem
	derived_obj.statmem();//正确，派生类的对象可以访问基类的静态成员
	statmem();//通过this进行访问
}
void test1(){
	Derived d1,d2;
	d1.f(d2);
}
//final关键字防止继承
class NoDerived final{};
class Last final:Base{};
//class Bad:NoDerived{};//错误，NoDerived被声明为final的
//class Bad2:Last{};//错误，Last被声明为final的
//不存在基类到派生类的转换，派生类到基类的转换只对指针和引用有效
void test2(){
	Base b;
	Derived d;
	Base* bp=&d;//正确，存在基类到派生类的隐式转换
	//Derived *dp=&b;//错误，不存在基类到派生类的隐式转换
	//Derived *dp=bp;//错误，尽管基类的指针或者引用绑定到派生类对象，也不能执行基类向派生类的转换
}
int main(){
	//test1();
	test2();
}

