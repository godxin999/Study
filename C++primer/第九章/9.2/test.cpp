/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/10 22:08:07
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <list>
#include <vector>
#include <deque>
#include <forward_list>
#include <array>
using namespace std;
//一个容器初始化为另一个容器的拷贝
void test1(){
	list<string> authors={"Alice","Bob","Tom"};//列表初始化，显式指定元素值，隐式指定容器大小
	vector<const char*> articles={"a","an","the"};
	list<string> list2(authors);
	//vector<string> words(articles);//错误，同种容器初始化为另一个容器的拷贝时，两个容器的类型及其元素类型必须匹配
	forward_list<string> words(articles.begin(),articles.end());//正确，通过迭代器范围来拷贝一个范围时，不要求容器类型相同，只要求拷贝的元素可以转换为要初始化的容器的元素类型即可，这里是const char*转换为string
}
//与顺序容器大小相关的构造函数，如果元素类型没有默认构造函数，就要显式提供元素初始值
void test2(){
	vector<int> ivec(10,-1);//10个整型，均为-1
	list<string> svec(10,"hi!");//10个string，均为"hi!"
	deque<string> svec2(10);//10个空string
	for(auto i:svec){
		cout<<i<<endl;
	}
}
//标准库array
void test3(){
	array<int,5> a;//array定义时要指定类型和大小
	array<int,5>::size_type i;//array<int,5>是一个类型
	array<int,5> b={1,2,3,4,5};//列表初始化
	array<int,5> c={1};//剩余元素值初始化为0
	for(auto i:b){
		cout<<i<<endl;//1 2 3 4 5
	}
	for(auto i:c){
		cout<<i<<endl;//1 0 0 0 0
	}
	b=c;//array如果类型相同可以直接拷贝和赋值
	for(auto i:b){
		cout<<i<<endl;//1 0 0 0 0
	}
}
//使用assign赋值
void test4(){
	list<string> names;
	vector<const char*> old;//const char*可以转换为string
	names.assign(old.cbegin(),old.cend());//将names中的元素替换为迭代器指定的范围中的元素的拷贝，注意传递给assign的迭代器不能是自身的迭代器
	list<string> sl(1);
	sl.assign(10,"Hi!");//用指定数目的元素替换容器中原有的元素
	for(auto i:sl){
		cout<<i<<endl;
	}
}
//使用swap交换
void test5(){
	vector<int> sv1={1};
	vector<int> sv2={2,3};
	swap(sv1,sv2);//非成员版本
	for(auto i:sv1){
		cout<<i<<endl;//2 3
	}
	for(auto i:sv2){
		cout<<i<<endl;//1 
	}
	sv1.swap(sv2);//成员版本
	for(auto i:sv1){
		cout<<i<<endl;//1
	}
	for(auto i:sv2){
		cout<<i<<endl;//2 3
	}
}
//max_size成员返回所能容纳元素的最大数量
void test6(){
	vector<int> v;
	cout<<v.max_size()<<endl;//4611686018427387903
}
//关系运算符
void test7(){
	vector<int> v1={1,3,5,7,9,12};
	vector<int> v2={1,3,9};
	vector<int> v3={1,3,5,7};
	vector<int> v4={1,3,5,7,9,12};
	cout<<(v1<v2)<<endl;//true，9>5
	cout<<(v1<v3)<<endl;//false，前四个元素相等，v1元素个数多，v1更大
	cout<<(v1==v4)<<endl;//true
	cout<<(v1==v2)<<endl;//false
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	test7();

}

