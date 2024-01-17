/*************************************************************************
	> File Name: 17.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/16 14:07:14
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <vector>
class T{
	std::vector<int> data_;
public:
	std::vector<int> data() const{
		return data_;
	}
};
T foo(){
	T t;
	return t;
}
void test1(){
	//auto &&__range=range_expr;为range-for的伪代码实现
	//foo().data()返回一个泛左值类型std::vector<int>&，但是foo()在表达式结束后就会被销毁，从而导致for循环中的访问无效对象
	for(auto& x:foo().data()){
		//...
	}
	//C++20支持初始化range-for
	for(T thing=foo();auto &x:thing.data()){
		//...
	}
}



int main(){
	test1();



	return 0;
}