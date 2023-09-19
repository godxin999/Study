/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/19 21:56:06
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <concepts>

template <typename T>
concept ConvertibleToString=std::is_convertible_v<T,std::string>;

class Person{
private:
	std::string name;
public:
	//写法一
	/*
	template <typename T>
	requires std::is_convertible_v<T,std::string>
	explicit Person(T&& n):name(std::forward<T>(n)){
		std::cout<<"Person(T&& n)"<<std::endl;
	}
	*/
	/*
	//写法二
	template <typename T>
	requires ConvertibleToString<T>
	explicit Person(T&& n):name(std::forward<T>(n)){
		std::cout<<"Person(T&& n)"<<std::endl;
	}*/
	//写法三
	template <ConvertibleToString T>
	explicit Person(T&& n):name(std::forward<T>(n)){
		std::cout<<"Person(T&& n)"<<std::endl;
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
	Person p2("name");
	Person p3(p1);
	Person p4(std::move(p1));
}





int main(){
	test1();

	return 0;
}