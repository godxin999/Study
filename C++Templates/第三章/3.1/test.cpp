/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/14 17:35:59
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <array>
#include <cassert>

template <typename T=int,std::size_t MaxSize=100>
class Stack{
private:
	std::array<T,MaxSize> elems;
	std::size_t numElems;
public:
	Stack();
	void push(T const&);
	void pop();
	const T& top() const;
	bool empty() const{
		return numElems == 0;
	}
	std::size_t size() const{
		return numElems;
	}
};
template <typename T,std::size_t MaxSize>
Stack<T,MaxSize>::Stack():numElems(0){}

template <typename T,std::size_t MaxSize>
void Stack<T,MaxSize>::push(const T& elem){
	assert(numElems < MaxSize);
	elems[numElems] = elem;
	++numElems;
}

template <typename T,std::size_t MaxSize>
void Stack<T,MaxSize>::pop(){
	assert(!elems.empty());
	--numElems;
}

template <typename T,std::size_t MaxSize>
const T& Stack<T,MaxSize>::top() const{
	assert(!elems.empty());
	return elems[numElems-1];
}

int main(){
	Stack<int,20> si20;
	Stack<int> si;
	Stack<> si2;
	Stack<std::string,10> ss10;
	si2.push(100);
	std::cout << si2.top() << std::endl;
	ss10.push("hello");
	std::cout << ss10.top() << std::endl;

	return 0;
}