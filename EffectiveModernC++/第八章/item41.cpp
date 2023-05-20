/*************************************************************************
	> File Name: item41.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/20 11:29:19
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <memory>
using namespace std;
//如果参数可拷贝且移动操作开销很低，考虑直接按值传递
class Bar{
public:
	void addElement(const string &e){//左值一次拷贝
		elements.push_back(e);
	}
	void addElement(string &&e){
		elements.push_back(std::move(e));//右值一次移动
	}
	template <class T>
	void addElement2(T &&e){
		elements.push_back(std::forward<T>(e));//同上
	}
	void addElement3(string e){
		elements.push_back(std::move(e));//左值一次拷贝一次移动，右值两次移动
	}
private:
	vector<string> elements;
};
class foo{
public:
	void setPtr(unique_ptr<string>&& ptr){
		p=std::move(ptr);
	}
	void setPtr2(unique_ptr<string> ptr){//会多出一次移动构造的开销
		p=std::move(ptr);
	}
private:
	std::unique_ptr<string> p;
};
class animal{};
class cat:public animal{};
void printAnimal(animal an){
	//按值传递错误，会造成切片
}
void test1(){
	cat c;
	printAnimal(c);
}
int main(){
	test1();
}