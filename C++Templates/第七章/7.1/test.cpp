/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/20 8:54:53
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <utility>

class myClass{
public:
	std::string s="myclass";
	myClass(const std::string& s):s(s){}
	myClass()=default;
	myClass(const myClass& rhs){
		s=rhs.s;
		std::cout<<"copy constructor"<<std::endl;
	}
	myClass(myClass&& rhs){
		s=std::move(rhs.s);
		std::cout<<"move constructor"<<std::endl;
	}
	myClass& operator=(const myClass& rhs){
		s=rhs.s;
		std::cout<<"copy assignment"<<std::endl;
		return *this;
	}
	myClass& operator=(myClass&& rhs){
		s=std::move(rhs.s);
		std::cout<<"move assignment"<<std::endl;
		return *this;
	}
	friend std::ostream& operator<<(std::ostream& os,const myClass &m){
		os<<m.s;
		return os;
	}
};

myClass returnMyClass(){
	return myClass();
}

//按值传递
template <typename T>
void print(T arg){
	std::cout<<arg<<std::endl;
}

void test1(){
	myClass m;
	print(m);//左值调用拷贝构造
	print(myClass("hello"));//编译器优化参数传递，不会调用拷贝构造函数
	print(returnMyClass());//编译器优化参数传递，不会调用拷贝构造函数
	print(std::move(m));//右值调用移动构造函数
}
//按值传递会导致类型退化
void test2(){
	const char a[]="hello";
	print(a);
}


int main(){
	//test1();
	test2();




}