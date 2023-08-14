/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/14 18:41:27
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <array>
//用auto作为非模板类型参数的类型
template <typename T,auto MaxSize>
class Stack{
public:
	using size_type =decltype(MaxSize);
private:
	std::array<T,MaxSize> elems;
	size_type numElems;
public:
	Stack();
	void push(T const&);
	void pop();
	const T& top() const;
	bool empty() const{
		return numElems == 0;
	}
	auto size() const{
		return numElems;
	}
};
template <typename T,auto MaxSize>
Stack<T,MaxSize>::Stack():numElems(0){}

template <typename T,auto MaxSize>
void Stack<T,MaxSize>::push(const T& elem){
	assert(numElems < MaxSize);
	elems[numElems] = elem;
	++numElems;
}

template <typename T,auto MaxSize>
void Stack<T,MaxSize>::pop(){
	assert(!elems.empty());
	--numElems;
}

template <typename T,auto MaxSize>
const T& Stack<T,MaxSize>::top() const{
	assert(!elems.empty());
	return elems[numElems-1];
}
void test1(){
	Stack<int,20u> si20;
	Stack<std::string,40> ss40;
	si20.push(7);
	std::cout<<si20.top()<<std::endl;
	auto sz1=si20.size();
	std::cout<<sz1<<std::endl;
	ss40.push("hello");
	std::cout<<ss40.top()<<std::endl;
	auto sz2=ss40.size();
	std::cout<<sz2<<std::endl;

	if(!std::is_same_v<decltype(sz1),decltype(sz2)>){
		std::cout<<"sz1 and sz2 have different types"<<std::endl;
	}
	else{
		std::cout<<"sz1 and sz2 have the same types"<<std::endl;
	}
}
//将字符串作为常量数组用于非类型模板参数
template <auto T>
class Message{
public:
	void print(){
		std::cout<<T<<std::endl;
	}
};
void test2(){
	Message<42> msg1;
	msg1.print();
	static const char s[]="hello";
	Message<s> msg2;
	msg2.print();
}
//decltype将表达式实例化为引用类型
template <decltype(auto) N>
class C{
public:
	void print(){
		if(std::is_same_v<decltype(N),int&>){
			std::cout<<"N's type is int&.";
		}
		else{
			std::cout<<"N's type isn't int&.";
		}
	}
};
int i=10;
void test3(){
	C<(i)> x;
	x.print();//N's type is int&.
}
int main(){
	test1();
	test2();
	test3();

	
	return 0;
}