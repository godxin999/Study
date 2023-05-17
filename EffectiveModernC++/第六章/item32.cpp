/*************************************************************************
	> File Name: item32.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/17 23:50:22
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <memory>
#include <functional>
#include <vector>
using namespace std;
//初始化捕获
class widget{
public:
	bool isValidated()const;
	bool isProcessed()const;
	bool isArchived()const;
};
void test1(){
	auto pw=std::make_unique<widget>();
	auto func=[pw=std::move(pw)]{//使用初始化捕获移交所有权
		return pw->isValidated()&&pw->isArchived();
	};
}
//C++11实现
class IsValAndArch{
public:
	using DataType=std::unique_ptr<widget>;
	explicit IsValAndArch(DataType&& ptr):pw(std::move(ptr)){}
	bool operator()()const{
		return pw->isValidated()&&pw->isArchived();
	}
private:
	DataType pw;
};
void test2(){
	auto func=IsValAndArch(std::make_unique<widget>());
}
//使用std::bind实现
void test3(){
	vector<double> data;
	auto func=bind([](const vector<double>& data){/*...*/},std::move(data));//把data作为参数传入lambda表达式中
}
//lambda表达式生成的类中的operator()是const的
void test4(){
	vector<double> data;
	//通过声明为mutable来允许lambda表达式对外部做修改
	auto func=bind([](vector<double>& data)mutable{/*...*/},std::move(data));
}
int main(){
	//test1();
	//test2();
	//test3();
	test4();


}