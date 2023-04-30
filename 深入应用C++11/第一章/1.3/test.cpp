/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/30 23:28:00
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//列表初始化
void test1(){
	int i_arr[3]{1,2,3};
	for(auto i:i_arr){
		cout<<i<<" ";
	}
	int *a=new int{123};
	double b=double{12.12};
	int *arr=new int[3]{1,2,3};
}
//列表初始化可以用于函数返回值
struct Foo{
	Foo(int,double){//构造函数
		cout<<"Foo(int,double)"<<endl;
	}
};
Foo func(){
	return {123,321.0};//等同于返回了一个Foo(123,321.0)
}
void test2(){
	func();
}
//
struct A{
	int x;
	int y;
}a={123,321};//C++98/03的聚合类型的初始化，采用拷贝的方式使用初始化列表中的值来初始化struct A中的成员
struct B{
	int x;
	int y;
	B(int,int):x(0),y(0){}
}b={123,321};//由于定义了一个构造函数，因此初始化以构造函数进行
void test3(){
	cout<<a.x<<" "<<a.y<<endl;//123 321
	cout<<b.x<<" "<<b.y<<endl;//0 0
}
//
struct C{
	int x;
	double y;
protected:
	int z;
};
struct D{
	int x;
	double y;
protected:
	static int z;
};
struct E{
	int x;
	double y;
	virtual void f(){}
};
struct Base{};
struct F:public Base{
	int x;
	double y;
};
struct G{
	int x;
	double y=0.0;
};
void test4(){
	//C c{1,2.5,1};//错误，C的z成员是一个受保护的非静态数据成员
	D d{1,2.5};//正确，D的z成员是一个受保护的静态成员，静态成员不能通过实例D的方式进行初始化，它的初始化要遵循静态成员的初始化方式
	//E e{1,2.5};//错误，E中有一个虚函数，所以E不为一个聚合类型
	//F f{1,2.5};//错误，F有一个基类Base，所以F不为一个聚合类型
	G g{1,2.5};//G中含有类内初始值，所以G不为一个聚合类型，但是可以使用{}进行初始化
}
//对于上述非聚合类的情况，如果要使用初始化列表，则需要定义一个构造函数
struct ST{
	int x;
	double y;
	virtual void f(){}
private:
	int z;
public:
	ST(int i,double j,int k):x(i),y(j),z(k){}
};
void test5(){
	ST s{1,2.5,2};
}
//聚合类的定义是非递归的
struct H{
	int x;
	double y;
private:
	int z;
};
struct I{//H不是聚合类，但是I是聚合类
	H h;
	int x;
	double y;
};
void test6(){
	I i{{},1,2.5};
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	test6();
}

