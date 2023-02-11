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
		//cout<<*v.begin()<<" "<<*--v.end()<<endl;//简洁获取首尾元素的引用
	}
	auto x=v.at(3);//at返回下标为n的引用，越界会抛出异常
	cout<<x<<endl;
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	test5();

}

