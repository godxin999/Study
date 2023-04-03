/*************************************************************************
	> File Name: item7.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/3 20:40:31
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <atomic>
using namespace std;
//三种初始化方式
void test1(){
	int x(0);
	int y=0;
	int z{0};//同int z={0}
}
//列表初始化可以用于指定非静态数据成员的初始值
class foo{
private:
	int x{0};
	int y=0;
	//int z(0);//错误，不能用小括号初始化指定默认值
};
//可以用列表初始化不可拷贝的对象
void test2(){
	atomic<int> ail{0};
	atomic<int> ail2(0);
	//atomic<int> ail3=0;//错误，不能使用等号初始化不可拷贝的对象
}
//列表初始化不允许内置类型进行隐式的变窄变换
void test3(){
	double x = 0, y = 0, z = 0;
	//int sum{x+y+z};//错误，三个double的和不能用于初始化int
	int sum2(x+y+z);//正确，表达式的值被截断为int
	int sum3=x+y+z;
}
//使用列表初始化可以正确调用无参构造函数
class A{
public:
	A(int){}
	A(){}
};
void test4(){
	A a1(10);
	//A a2();//错误，使用无参数的构造函数构造对象会被解析为函数声明
	A a3{};//正确，调用无参构造函数
}
//如果含有以初始化列表为参数的构造函数，那么在使用初始化列表初始化时会优先匹配该函数
class B{
public:
	B(int,bool){
		cout<<"B(int,bool)"<<endl;
	}
	B(int,double){
		cout<<"B(int,double)"<<endl;
	}
};
void test5(){
	B b1(10,true);//B(int,bool)
	B b2{10,true};//B(int,bool)
	B b3(10,5.0);//B(int,double)
	B b4{10,5.0};//B(int,double)
}
class C{
public:
	C(const C&){
		cout<<"C(const C&)"<<endl;
	}
	C(C&&)noexcept{
		cout<<"C(C&&)"<<endl;
	}
	C(int,bool){
		cout<<"C(int,bool)"<<endl;
	}
	C(int,double){
		cout<<"C(int,double)"<<endl;
	}
	C(initializer_list<long double>){
		cout<<"C(initializer_list<long double>)"<<endl;
	}
	operator float()const{
		cout<<"operator float()const"<<endl;
		return i;
	}
	float i = 0;
};
void test6(){
	C c1(10,true);//C(int,bool)
	C c2{10,true};//C(initializer_list<long double>)
	C c3(10,5.0);//C(int,double)
	C c4{10,5.0};//C(initializer_list<long double>)
	C c5(c4);//C(const C&)
	C c6{c4};//C(const C&)
	float c7(std::move(c4));//operator float()const，C类型转换为float
	C c7_(std::move(c5));//C(C&&)
	C c8_{ std::move(c6) }; //C(C&&)
	C c8{std::move(c7)};//C(initializer_list<long double>)，float转换为long double
}
//编译器优先匹配含有初始化列表参数的构造函数，但是该构造函数要求变窄变换，故编译器报错
class D{
public:
	D(int,bool){
		cout<<"D(int,bool)"<<endl;
	}
	D(int,double){
		cout<<"D(int,double)"<<endl;
	}
	D(initializer_list<bool>){
		cout<<"D(initializer_list<bool>)"<<endl;
	}
};
void test7(){
	//D d{10,5.0};
	//error: constant expression evaluates to 10 which cannot be narrowed to type 'bool' [-Wc++11-narrowing]
	//error: type 'double' cannot be narrowed to 'bool' in initializer list [-Wc++11-narrowing]
}
//当无法把实参转换为初始化列表时，编译器会回到正常的函数决议流程中
class E{
public:
	E(){
		cout<<"E()"<<endl;
	}
	E(int,bool){
		cout<<"E(int,bool)"<<endl;
	}
	E(int,double){
		cout<<"E(int,double)"<<endl;
	}
	E(initializer_list<string>){
		cout<<"E(initializer_list<string>)"<<endl;
	}
};
void test8(){
	E e{10,5.0};//E(int,double)
}
//空大括号会调用默认构造函数
void test9(){
	E e;//E()
	E e2{};//E()
}
//空花括号实参会被认为是一个空的初始化列表
void test10(){
	E e({});//E(initializer_list<string>)
	E e2{{}};//E(initializer_list<string>)
}
//列表初始化的一些其他影响
void test11(){
	vector<int> v1(10,20);
	cout<<v1.size()<<endl;//10
	vector<int> v2{10,20};
	cout<<v2.size()<<endl;//2
}

int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7();
	//test8();
	//test9();
	//test10();
	test11();
}
