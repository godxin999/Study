/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/4 16:03:03
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//派生类中删除的拷贝控制和基类的关系
class B{
public:
	B()=default;
	B(const B&)=delete;
};
class D:public B{
public:

};
void test1(){
	D d;//正确，D的合成默认构造函数使用B的默认构造函数
	//D d2(d);//错误，D的合成拷贝构造函数是delete的
	//D d3(move(d));//错误，隐式使用了D的被删除的拷贝构造函数
}
//派生类的拷贝控制成员
class Base{
public:
	Base()=default;
	Base(const Base&)=default;
	Base(Base&&)=default;
	Base& operator=(const Base&)=default;
	Base& operator=(Base&&)=default;
	virtual ~Base()=default;
private:
	int i=0;
};
class Derived:public Base{
public:
	Derived()=default;
	Derived(const Derived& d):Base(d),size(0){}//派生类的拷贝构造函数
	Derived(Derived&& d):Base(move(d)),size(d.size){}//派生类的移动构造函数
	Derived& operator=(const Derived& d){//派生类的拷贝赋值运算符
		if(this==&d){
			return *this;
		}
		Base::operator=(d);//调用基类的拷贝赋值运算符
		size=d.size;
		return *this;
	}
	Derived& operator=(Derived&& d){//派生类的移动赋值运算符
		if(this==&d){
			return *this;
		}
		Base::operator=(move(d));//调用派生类的移动赋值运算符
		size=d.size;
		return *this;
	}
	~Derived(){
		//定义清除派生类成员的位置
		//Base::~Base将会自动调用
	}
private:
	int size=0;
};
void test2(){
	Derived d1;
	d1=d1;
	d1=move(d1);
}
class c1{
public:
	c1()=default;
	explicit c1(int a,int b=1){}
};
class c2:public c1{
private:
	using c1::c1;//继承构造函数，并且不改变构造函数的访问属性
	//c1的默认构造函数不被继承，但是会合成默认构造函数，这个函数会调用基类的默认构造函数
	//explicit和constexpr均会被继承，所以这里只继承了explicit c1(int a,int b=1){}，并生成两个构造函数，分别为：
	//explicit c2(int a):c1(a,1){}
	//explicit c2(int a,int b):c1(a,b){}
};
void test3(){
	c2 a;//调用c2合成的默认构造函数
	c2 b(1);//调用继承的拷贝构造函数
	c2 c(1,1);
}


int main(){
	//test1();
	//test2();
	test3();



}

