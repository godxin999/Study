/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/13 17:11:08
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <deque>
#include <cassert>

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
//类模板别名
using intStack=Stack<int>;
//别名模板
template <typename T>
using dequeStack=Stack<T,std::deque<T>>;
//使用别名模板来省略typename
template <typename T>
struct identity{
	typedef T type;
};
template <typename T>
using type_t=typename identity<T>::type;

int main(){

}