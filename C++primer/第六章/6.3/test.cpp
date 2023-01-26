/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/1/26 13:15:59
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
//void函数可以返回一个另一个void函数
void func2(){
	cout<<"func2"<<endl;
	return ;
}
void func1(){
	cout<<"func1"<<endl;
	return func2();
}
void test1(){
	return func1();
}
//点运算符和调用运算符满足左结合律，且优先级相同
const string&shorterstring(const string& s1,const string &s2){
	return s1.size()<=s2.size()?s1:s2;
}
void test2(){
	string s1("apple");
	string s2("banana");
	auto sz=shorterstring(s1,s2).size();//点运算符获取函数返回值的size成员
	cout<<sz<<endl;
}
//调用返回引用的函数得到左值
char &get_val(string &s1,string::size_type ix){
	return s1[ix];
}
void test3(){
	string s("a value");
	cout<<s<<endl;
	get_val(s,0)='A';
	cout<<s<<endl;
}
//对于标准库类型，可以使用初始化列表返回值
vector<int> t_list(int i){
	if(i==1){
		return {1};
	}
	else if(i==2){
		return {1,2};
	}
	else return {1,2,3};
}
void test4(){
	auto sz=t_list(3).size();
	cout<<sz<<endl;
}
//函数返回数组指针
//第一种:使用类型别名
//typedef int arr[10];
using arr=int[10];
int b[10]={0,1,2,3,4,5,6,7,8,9};
arr *f(int(&a)[10]){//不能返回局部变量的指针或者引用，所以传入的参数是&
	return &a;
}
void test5(){
	int (*p)[10]=f(b);
	for(auto i:*p){
		cout<<i<<endl;
	}
}
//第二种:不使用类型别名直接定义
int (*f2(int(&a)[10]))[10]{
	return &a;
}
void test6(){
	int (*p)[10]=f2(b);
	for(auto i:*p){
		cout<<i<<endl;
	}
}
//第三种:使用尾置返回类型
auto f3(int(&a)[10])->int(*)[10]{//->之后的是真正的返回类型
	return &a;
}
void test7(){
	int (*p)[10]=f3(b);
	for(auto i:*p){
		cout<<i<<endl;
	}
}
//第四种:使用decltype
decltype(b) *f4(int(&a)[10]){//decltype传入数组返回数组类型，加上*即为数组的指针
	return &a;
}
void test8(){
	int (*p)[10]=f4(b);
	for(auto i:*p){
		cout<<i<<endl;
	}
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7();
	test8();
}

