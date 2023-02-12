/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/12 3:04:41
 ************************************************************************/
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <list>
#include <forward_list>
#include "C:\Users\admin\Desktop\Study\C++primer\第七章\Sales_data.h"
using namespace std;
//insert
void test1(){
	vector<string> svec;
	list<string> slist;
	slist.insert(slist.begin(),"Hello!");//等价于push_front
	svec.insert(svec.begin(),"hello!");//vector没有push_front操作，所以使用insert操作来做到插入到头部
}
//插入范围内元素
void test2(){
	vector<string> svec;
	list<string> slist;
	svec.insert(svec.end(),10,"Anna");//向svec末尾插入10个元素，并初始化为Anna
	for(auto i:svec){
		cout<<i<<endl;
	}
	vector<string> v={"a","b","c","d"};
	slist.insert(slist.begin(),v.end()-2,v.end());//把v中倒数两个元素插入到slist头部
	for(auto i:slist){
		cout<<i<<endl;
	}
	slist.insert(slist.end(),{"e","f","g","h"});//把初始化列表中的元素插入到slist的末尾
	for(auto i:slist){
		cout<<i<<endl;
	}
}
//使用insert返回值
void test3(){
	list<string> lst;
	string word;
	auto iter=lst.begin();
	while(cin>>word){
		iter=lst.insert(iter,word);//insert返回第一个新加入元素的迭代器，可以在相同的位置不断插入元素，这里等价于调用push_front
	}
	for(auto i:lst){
		cout<<i<<endl;
	}
}
//使用emplace
void test4(){
	vector<Sales_data> c;
	c.emplace_back("111",25,15);//emplace操作可以通过参数构造元素，而调用push_back会对元素进行拷贝(临时对象)，然后压入容器中
	c.emplace_back();//使用默认构造函数
	c.emplace(c.end(),"123456");//使用单参数构造函数
	for(auto i:c){
		cout<<i.isbn()<<endl;
	}
}
//访问元素
void test5(){
	vector<int> v{1,2,3,4,5};
	if(!v.empty()){
		cout<<v.front()<<" "<<v.back()<<endl;//直接获取首尾元素的引用
		//cout<<*v.begin()<<" "<<*--v.end()<<endl;//间接获取首尾元素的引用
	}
	auto x=v.at(3);//at返回下标为n的引用，越界会抛出异常
	cout<<x<<endl;
}
//访问成员函数返回的是引用
void test6(){
	vector<int> v{1,2,3,4,5};
	if(!v.empty()){
		v.back()=42;//最后一个元素赋值为42
		for(auto i:v){
			cout<<i<<endl;
		}
		auto &i=v.front();//首元素绑定到i
		i=1024;
		for(auto i:v){
			cout<<i<<endl;
		}
		auto ii=v.front();//没有用引用类型绑定，所以无法修改v中的值
		ii=0;
		for(auto i:v){
			cout<<i<<endl;
		}
	}
}
//下标操作和安全的随机访问
void test7(){
	vector<int> v;
	//cout<<v[0]<<endl;//运行时错误
	try{
		cout<<v.at(0);//会抛出越界异常
	}
	catch(out_of_range err){
		cout<<err.what()<<endl;
	}
}
//删除元素
void test8(){
	vector<int> v={0,1,2,3,4,5,6,7,8,9};
	auto it=v.begin();
	while(it!=v.end()){
		if(*it&1){//奇数就删除
			it=v.erase(it);//erase返回删除的最后一个元素之后位置的迭代器
		}
		else ++it;
	}
	for(auto i:v){
		cout<<i<<endl;
	}
	cout<<endl;
	auto it1=v.begin();
	auto it2=v.begin()+2;
	it1=v.erase(it1,it2);//list的迭代器不支持加减运算
	for(auto i:v){
		cout<<i<<endl;
	}
	cout<<endl;
	v.clear();//清空元素
	//v.erase(v.begin(),v.end());//等价操作
	for(auto i:v){
		cout<<i<<endl;
	}
}
//forward_list操作
void test9(){
	forward_list<int> flst={0,1,2,3,4,5,6,7,8,9};
	auto pre=flst.before_begin();//首前迭代器
	auto cur=flst.begin();
	while(cur!=flst.end()){
		if(*cur&1){
			cur=flst.erase_after(pre);//返回删除元素之后位置的迭代器
		}
		else{
			pre=cur;
			++cur;
		}
	}
	for(auto i:flst){
		cout<<i<<endl;
	}
}
//改变容器大小
void test10(){
	list<int> ilst(2,9);
	for(auto i:ilst){
		cout<<i<<endl;//9 9
	}
	cout<<endl;
	ilst.resize(3);
	for(auto i:ilst){
		cout<<i<<endl;//9 9 0
	}
	cout<<endl;
	ilst.resize(5,-1);
	for(auto i:ilst){
		cout<<i<<endl;//9 9 0 -1 -1
	}
	cout<<endl;
	ilst.resize(1);
	for(auto i:ilst){
		cout<<i<<endl;//9
	}
	cout<<endl;
}
//更新迭代器防止失效
void test11(){
	vector<int> v={0,1,2,3,4,5,6,7,8,9};
	auto it=v.begin();
	while(it!=v.end()){
		if(*it&1){
			it=v.insert(it,*it);//利用返回迭代器的操作进行更新
			it+=2;//定位到未处理元素
		}
		else{
			it=v.erase(it);//同理更新
		}
	}
	for(auto i:v){
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
	//test8();
	//test9();
	//test10();
	test11();
}

