/*************************************************************************
	> File Name: 7.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/14 9:53:26
 ************************************************************************/

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <future>
#include <format>

//使用mutable改变lambda表达式的常量性
void test1(){
	int x=5,y=8;
	auto foo=[x,&y]()mutable{
		x+=1;
		y+=2;
		std::cout<<std::format("x = {} , y = {}\n",x,y);
		return x*y;
	};
	foo();
	std::cout<<std::format("x = {} , y = {}\n",x,y);
	foo();
	std::cout<<std::format("x = {} , y = {}\n",x,y);
	
	//x = 6 , y = 10
	//x = 5 , y = 10
	//x = 7 , y = 12 在lambda内修改x的值不会影响外面的值，但是会影响下一次调用中x的值
	//x = 5 , y = 12
	
}
//使用lambda捕获this
class A{
public:
	void print(){
		std::cout<<x<<std::endl;
	}
	void test(){
		auto foo=[this](){
			x=5;
			print();
		};
		foo();
	}
private:
	int x;
};
void test2(){
	A a;
	a.test();
}
//无状态lambda表达式可以隐式转换为函数指针
void f(void(*)()){
	std::cout<<"f(void(*)())"<<std::endl;
}
void test3(){
	f([](){
		std::cout<<"lambda"<<std::endl;
	});
}
//广义捕获
void test4(){
	int x=5;
	auto foo=[r=x+1](){
		std::cout<<r<<std::endl;
	};
	foo();
	std::string str="hello";
	auto foo2=[s=std::move(str)](){
		std::cout<<s+"world"<<std::endl;
	};
	foo2();
}
//C++17支持捕获*this，即this对象的副本，防止源对象被析构导致问题
class work{
private:
	int val{0};
public:
	std::future<int> spawn(){
		return std::async([=,*this](){
			return val;
		});
	}
};
void test5(){
	work w;
	auto fut=w.spawn();
	fut.wait();
	std::cout<<fut.get()<<std::endl;
}
//C++20使用[=,this]显式捕获this指针，替代以往使用[=]隐式捕获this指针
struct Foo{
	int n=0;
	void f(int a){
		auto func=[=,this](int k){
			return n+a*k;
		};
		std::cout<<func(5)<<std::endl;
	}
};
void test6(){
	Foo foo;
	foo.f(5);
}
//模板lambda
auto func=[]<typename T>(std::vector<T> vec){
	for(auto i:vec){
		std::cout<<i<<std::endl;
	}
};
auto func2=[]<typename T>(const T& x){
	T copy = x;
	using Iter=typename T::iterator;
	for(Iter it=copy.begin();it!=copy.end();++it){
		std::cout<<*it<<std::endl;
	}
};
void test7(){
	std::vector<int> vec{1,2,3,4,5};
	func(vec);
	func2(vec);
}
//C++20支持无状态lambda的构造和赋值
auto greater=[](auto x,auto y){
	return x>y;
};
void test8(){
	//C++17中，因为无状态lambda表达式无法构造，所以decltype(greater)编译失败
	std::map<std::string,int,decltype(greater)> m1,m2;
	//C++17中，因为无状态lambda表达式无法赋值，所以m1=m2编译失败
	m1=m2;
}

int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7();
	test8();
	return 0;
}