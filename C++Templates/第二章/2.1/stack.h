/*************************************************************************
	> File Name: stack.h
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/11 20:23:33
 ************************************************************************/

#include <iostream>
#include <vector>
#include <cassert>

template <typename T>
class Stack{
private:
	std::vector<T> elems;
public:
	Stack()=default;//在类模板内部可以省略<T>
	Stack(const Stack&)=default;
	Stack& operator=(const Stack&)=default;
	//对于没有提供operator<<运算符的元素，调用该函数会报错
	void printOn(std::ostream& os)const{
		for(const T& elem:elems){
			os<<elem<<" ";
		}
	}
	void push(const T&);
	void pop();
	const T& top() const;
	bool empty() const{
		return elems.empty();
	}
};

template <typename T>
void Stack<T>::push(const T& elem){
	elems.push_back(elem);
}

template <typename T>
void Stack<T>::pop(){
	assert(!elems.empty());
	elems.pop_back();
}

template <typename T>
const T& Stack<T>::top() const{
	assert(!elems.empty());
	return elems.back();
}

template <typename T>
bool operator==(const Stack<T>& lhs, const Stack<T>& rhs){
	return lhs.elems == rhs.elems;
}