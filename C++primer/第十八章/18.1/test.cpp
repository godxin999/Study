/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/20 9:57:33
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <memory>
#include <vector>
using namespace std;

//函数try语句块
template <typename T>
class Blob{
public:
	Blob(initializer_list<T> il);
private:
	shared_ptr<vector<T>> data;
};
template <typename T>
Blob<T>::Blob(initializer_list<T> il)try :
	data(make_shared<vector<T>> (il)){
	//将构造函数写成函数try语句块来处理构造函数初始值异常
}
catch(const bad_alloc &e){
	//handle_out_of_memory
}
//异常说明的实参
void recoup(int) noexcept(true){}//recoup不会抛出异常
void alloc(int) noexcept(false){}//alloc可能会抛出异常
//noexcept运算符
void g()noexcept{}
void test1(){
	cout<<boolalpha<<noexcept(recoup(100));//true，因为recoup不会抛出异常，所以该表达式为真
}
void f()noexcept(noexcept(g())){
	//f和g的异常声明相同，当g承诺了不会抛出异常，那么f也不会抛出异常，反之同理。
}
//异常说明和函数指针
void test2(){
	void (*pf1)(int)noexcept=recoup;//recoup和pf1均声明不会抛出异常
	void (*pf2)(int)=recoup;//recoup不会抛出异常，但是pf2可能会抛出异常
	//pf1=alloc;//错误，pf1已经声明不会抛出异常，而alloc可能会抛出异常
	pf2=alloc;//alloc和pf2都有可能会抛出异常
}
//派生的虚函数需要和基类虚函数的异常说明一致
class Base{
public:
	virtual double f1(double)noexcept;
	virtual int f2()noexcept(false);
	virtual void f3();
};
class Derived:public Base{
public:
	//double f1(double);//错误，Base::f1承诺不会抛出异常
	int f2()noexcept(false);//正确，和Base::f2的异常声明一致
	void f3()noexcept;//正确，对派生类的f3做了更严格的限定
};


int main(){
	//test1();
	test2();
}

