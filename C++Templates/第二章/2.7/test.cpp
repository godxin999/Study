/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/13 13:48:25
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <deque>
#include <cassert>
//类模板默认参数
template <typename T,typename Cont=std::vector<T>>
class Stack{
private:
	Cont elems;
public:
	void push(T const&);
	void pop();
	const T& top() const;
	bool empty() const{
		return elems.empty();
	}
};
template <typename T,typename Cont>
void Stack<T,Cont>::push(const T& elem){
	elems.push_back(elem);
}
template <typename T,typename Cont>
void Stack<T,Cont>::pop(){
	assert(!elems.empty());
	elems.pop_back();
}
template <typename T,typename Cont>
const T& Stack<T,Cont>::top() const{
	assert(!elems.empty());
	return elems.back();
}
int main(){
	Stack<int> si;
	Stack<int,std::deque<int>> sdi;
	si.push(1);
	std::cout<<si.top()<<std::endl;
	si.pop();
	sdi.push(2);
	std::cout<<sdi.top()<<std::endl;
	sdi.pop();
}