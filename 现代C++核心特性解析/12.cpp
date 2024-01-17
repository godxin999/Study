/*************************************************************************
	> File Name: 12.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/15 15:19:29
 ************************************************************************/

#include <fstream>
#include <ios>
#include <iostream>
//委托构造函数
//首先执行代理构造函数的初始化列表，然后执行代理构造函数的函数体，最后执行委托构造函数的函数体
//委托构造函数不能和初始化列表同时使用，即不能对数据成员和基类进行初始化
class X{
public:
	X():X(0){std::cout<<"3"<<std::endl;}
	X(int i):X(i,0.){std::cout<<"2"<<std::endl;}
	X(double j):X(0,j){}
	X(int i, double j):a(i),b(j){std::cout<<"1"<<std::endl;}
private:
	int a;
	double b;
};
void test1(){
	X x;
	//1
	//2
	//3
}
//如果在代理构造函数完成后，委托构造函数主体抛出了异常，那么会自动调用该类型的析构函数
class X2{
public:
	X2():X2(0,0.){throw 1;}
	X2(int i, double j):a(i),b(j){}
	~X2(){std::cout<<"~X2()"<<std::endl;}
private:
	int a;
	double b;
};
void test2(){
	try{
		X2 x;//~X2()
	}
	catch(...){

	}
}
//将代理构造函数写成一个函数模板
#include <vector>
#include <list>
#include <deque>
class foo{
	template<typename T>
	foo(T first,T last):l_(first,last){}
	std::list<int> l_;
public:
	foo(std::vector<int>& v):foo(v.begin(),v.end()){}
	foo(std::deque<int>& d):foo(d.begin(),d.end()){}
};
void test3(){
	std::vector<int> v{1,2,3,4,5};
	std::deque<int> d{1,2,3,4,5};
	foo f1(v);
	foo f2(d);
}
//捕获委托构造函数的异常
class X3{
public:
	X3() try :X3(0){}
	catch(int e){
		std::cout<<"catch: "<<e<<std::endl;
		throw 3;
	}
	X3(int a) try :X3(a,0.){}
	catch(int e){
		std::cout<<"catch: "<<e<<std::endl;
		throw 2;
	}
	X3(int a, double b):a(a),b(b){throw 1;};
private:
	int a;
	double b;
};
void test4(){
	try{
		X3 x;
	}
	catch(int e){
		std::cout<<"catch: "<<e<<std::endl;
	}
	//catch: 1
	//catch: 2
	//catch: 3
}
//当参数会在函数体内使用时委托参数较少的构造函数
/*
basic_fstream();
explicit basic_fstream(const char* s,std::ios_base::openmode mode=std::ios_base::in|std::ios_base::out);
basic_fstream::basic_fstream(const char* s,std::ios_base::openmode mode):basic_fstream(){//委托无参构造函数完成最基本的初始化工作
	if(open(s,mode)==0){
		setstate(failbit);
	}
}
*/
int main(){
	//test1();
	//test2();
	//test3();
	test4();

	return 0;
}