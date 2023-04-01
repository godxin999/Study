/*************************************************************************
	> File Name: item5.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/1 11:35:16
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <iterator>
#include <vector>
#include <unordered_map>
using namespace std;
//auto变量必须初始化
void test1(){
	//auto x;//错误，auto变量必须初始化
	auto y=10;
}
//使用auto替代typename指定
template <typename It>
void dwim(It b,It e){
	while(b!=e){
		//typename std::iterator_traits<It>::value_type currValue=*b;
		auto currValue=*b;
	}
}
//使用auto得到匿名函数而不是使用std::function
void test2(){
	auto f=[](const auto &p1,const auto &p2){
		return *p1<*p2; 
	};
	//使用std::function更耗空间、速度更慢且需要重复写很多形参类型
}
//使用auto避免一些问题
void test3(){
	vector<int> v;
	//unsigned sz=v.size();//32位机中，std::vector<int>::size_type是和unsigned一样的类型，但是到64位机种，std::vector<int>::size_type是64位的，而unsigned为32位，可能会导致问题
	cout<<sizeof(vector<int>::size_type)<<endl;//8
	auto sz=v.size();//使用auto避免错误
}
void test4(){
	unordered_map<string,int> m;
	/*
	for(const pair<string,int> &p : m){//unordered_map的key是一个常量，所以pair的类型应该为 pair<const string,int>
		//...
	}
	*/
	for(const auto &p : m){//使用auto避免错误
		//...
	}
}
int main(){
	//test1();
	//test2();
	//test3();
	test4();
}