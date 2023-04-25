/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/25 20:40:41
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//C++11支持了Foo<A<int>>
template <int N>
struct Foo{};
void test1(){
	//Foo<100 >> 2> x1;//C++11报错
	Foo<(100 >> 2)> x2;
}
//using别名定义模板别名
template <typename T>
using func_t=void(*)(T,T);
template <typename T>
using type_t=T;
void test2(){
	func_t<int> xx_2;
	type_t<int> i;//type_t<int>等价于int
}
//C++11支持了函数的默认模板参数，且不需要把默认参数写到参数表最后的要求
template <typename T=int>
void func(){
	cout<<typeid(T).name()<<endl;
}
template <typename T=int>
class A{
public:
	void show(){
		cout<<typeid(T).name()<<endl;
	}
};
//对于函数模板，如果全部使用默认实参，则可以当作普通函数调用，对于类模板，如果全部使用默认实参，仍需要在模板名后跟随<>来实例化
void test3(){
	func();//int
	func<double>();//double
	A<> a;
	a.show();//int
	A<double> ad;
	ad.show();//double
}
//当默认模板参数和模板参数自动推导同时使用时，如果函数模板无法推导出参数类型，则使用默认模板参数
template <typename T>
struct identity{//identity外敷模板禁用了形参val的模板自动推导
	typedef T type;
};
template <typename T=int>
void f(typename identity<T>::type val,T=0){
	cout<<typeid(val).name()<<endl;
	cout<<typeid(T).name()<<endl;
}
void test4(){
	f(123);//int int，因为f指定了默认类型，因此f的val参数为int类型
	f(123,123.0);//double double，因为第二个参数为double类型，T被优先推导为double，所以此时val为都变了类型
}
int main(){
	//test1();
	//test2();
	//test3();
	test4();
}