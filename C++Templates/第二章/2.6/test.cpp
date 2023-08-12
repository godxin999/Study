/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/12 18:39:38
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include "..\2.1\stack.h"
//部分特例化，应对部分特殊情况
template<typename T>
class Stack<T*>{
private:
	std::vector<T*> elems;
public:
	void push(T*);
	T* pop();
	T* top() const;
	bool empty() const{
		return elems.empty();
	}
};

template <typename T>
void Stack<T*>::push(T* elem){
	elems.push_back(elem);
}
//特例化后函数接口可能不同，这里返回一个指针
template <typename T>
T* Stack<T*>::pop(){
	assert(!elems.empty());
	T* p=elems.back();
	elems.pop_back();
	return p;
}

template <typename T>
T* Stack<T*>::top() const{
	assert(!elems.empty());
	return elems.back();
}

int main(){
	Stack<int*> spi;
	spi.push(new int(42));
	std::cout<<*spi.top()<<std::endl; 
	delete spi.pop();
}