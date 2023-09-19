/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/19 10:24:03
 ************************************************************************/

#include <algorithm>
#include <iostream>

//F.15 优先采用约定俗成的参数传递方式
//参数传递方向       拷贝开销低或无法拷贝       移动开销低到中，或者未知       移动开销高
//入                func(X)					  func(const X&)			func(const X&)
//入并保留拷贝       func(X)			       func(X)			         func(const X&)
//入并移入			 func(X&&)			       func(X&&)			     func(X&&)
//入/出				 func(X&)			       func(X&)			         func(X&)
//出				 X func()		           X func()					 func(X&)

//拷贝开销低或无法拷贝：int std::unique_ptr
//移动开销低：std::vector<T> std::string
//移动开销中：std::array<std::vector>或者BigPOD，POD即传统数据类型(无析构函数、构造函数及虚函数的类)
//移动开销未知：模板
//移动开销高：BigPOD[] std::array<BigPOD>
//在不进行内存分配的前提下，1000字节左右的操作数据中等开销

//F.16 对于入参，拷贝开销低的类型按值传递，其他类型则使用const&传递
//如果 sizeof(par)<=2*sizeof(void*)，则按值传递
//如果 sizeof(par)>2*sizeof(void*)，则使用const&传递

void f1(const std::string& s){
	//总是低开销
}
void f2(std::string s){
	//潜在的高昂开销
}
void f3(int x){
	//无可匹敌
}
void f4(const int& x){
	//在f4()内部访问时有额外开销
}

//F.19 对于转发参数，要使用T&&传递，并且只std::forward该参数

#include <utility>
#include <string>

template <typename T,typename... Args>
T create(Args&&... args){
	return T(std::forward<Args>(args)...);
}
struct MyType{
	MyType(int,double,bool){}
};

void test1(){
	int five=5;
	int myFive=create<int>(five);
	int myFive2=create<int>(5);
	int myZero=create<int>();
	MyType myType=create<MyType>(myZero,5.5,true);
}
//转发和变参模板的结合时C++中典型的创建模式，比如std::make_unique

//F.17 对于入/出型参数使用非const的引用来传递
#include <vector>
std::vector<int> vi{1,2,3,4,5};
void modifyVector(std::vector<int>& v){
	v.push_back(6);
	v.insert(v.end(),{7,8,9,10});
}
void test2(){
	modifyVector(vi);
	for(const auto& i:vi){
		std::cout<<i<<" ";
	}
}

//F.20 对于出的输出值，优先使用返回值而非输出参数
struct Foo{
	Foo()=default;
	Foo(const Foo& f){
		std::cout<<"copy ctor"<<std::endl;
	}
	Foo& operator=(const Foo& f){
		std::cout<<"copy operator="<<std::endl;
		return *this;
	}
};
Foo func(){
	return Foo{};//C++会采用RVO返回值优化来避免拷贝(构造函数可以不可见)
}
Foo func2(){
	Foo f;
	return f;//C++会采用NRVO具名返回值优化来避免拷贝，但是仍允许返回拷贝值f(要求构造函数可见)
}
void test3(){
	Foo f1=func();//不会发生拷贝
	Foo f2=func2();//不会发生拷贝
}

//F.21 要返回多个出值，优先考虑返回结构体或者元组
#include <tuple>
#include <set>

void test4(){
	std::cout<<std::endl;
	std::set<int> si;
	std::set<int>::iterator it;
	bool inserted=false;
	std::tie(it,inserted)=si.insert(10);
	if(inserted){
		std::cout<<"10 is successfully inserted"<<std::endl;
	}
	auto [it2,inserted2]=si.insert(20);
	if(inserted2){
		std::cout<<"20 is successfully inserted"<<std::endl;
	}
	std::cout<<std::endl;
}


int main(){
	//test1();
	//test2();
	//test3();
	test4();


}