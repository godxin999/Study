/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/24 17:04:49
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//通过模板来推迟计算
template <typename T>
class Cont{
private:
	T* elems;
public:
	/*
	std::conditional_t<std::is_move_constructible_v<T>,T&&,T&> foo(){
		//error: incomplete type 'Node' used in type trait expression
		//因为std::is_move_constructible_v<T>是在编译期间进行判断的，而此时T是不完全类型
	}
	*/
	//为了解决这个问题，需要使用一个成员模板来代替现有foo的定义，这样就可以将std::is_move_constructible_v<T>的求值推迟到实例化时
	template <typename D=T>
	std::conditional_t<std::is_move_constructible_v<D>,T&&,T&> foo(){

	}
};
struct Node{
	std::string val;
	Cont<Node> next;
};

int main(){
	Node *p = new Node;	




	return 0;
}