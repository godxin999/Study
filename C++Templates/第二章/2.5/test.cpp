/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/12 18:52:45
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <deque>
#include "..\2.1\stack.h"

template<>
class Stack<std::string>{
private:
	std::deque<std::string> elems;
public:
	void push(const std::string&);
	void pop();
	const std::string& top()const;
	bool empty()const{
		return elems.empty();
	}
};
void Stack <std::string>::push(const std::string& elem){
	elems.push_back(elem);
}
void Stack<std::string>::pop(){
	assert(!elems.empty());
	elems.pop_back();
}
const std::string& Stack<std::string>::top()const{
	assert(!elems.empty());
	return elems.back();
}
int main(){
	Stack<std::string> ss;
	ss.push("hello");
	std::cout<<ss.top()<<std::endl;
	ss.pop();
}