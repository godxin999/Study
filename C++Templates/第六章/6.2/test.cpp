/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/19 20:18:24
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <utility>

class Person{
private:
	std::string name;
public:
	/*
	explicit Person(const std::string& n):name(n){
		std::cout<<"Person(const std::string& n)"<<std::endl;
	}
	explicit Person(std::string&& n):name(std::move(n)){
		std::cout<<"Person(std::string&& n)"<<std::endl;
	}
	*/
	template<typename T>
	explicit Person(T&& n):name(std::forward<T>(n)){
		//传入C风格字符串时类型会被推导为const char[N]，从而导致错误
		//且该模板比拷贝构造函数更优先，从而导致错误
	}
	Person(const Person& p):name(p.name){
		std::cout<<"Person(const Person& p)"<<std::endl;
	}
	Person(Person&& p):name(std::move(p.name)){
		std::cout<<"Person(Person&& p)"<<std::endl;
	}
};
void test1(){
	std::string s="name";
	Person p1(s);
	//Person p2("temp");
	//Person p3(p1);
	Person p4(std::move(p1));
}

int main(){
	test1();
	return 0;
}