/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/22 10:56:27
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <list>
#include <iterator>
#include <numeric>
using namespace std;
//插入迭代器
void test1(){
	vector<int> v;
	auto it=back_inserter(v);
	for(int i=1;i<=5;i++){
		int x;
		cin>>x;
		*it=x;//插入操作
	}
	for(auto i:v){
		cout<<i<<" ";
	}
	cout<<endl;
	list<int> l1={1,2,3,4};
	list<int> l2,l3;
	copy(l1.cbegin(),l1.cend(),front_inserter(l2));
	copy(l1.cbegin(),l1.cend(),inserter(l3,l3.begin()));//其中inserter把新元素插入到第二个参数所指的元素之前并指向新元素，然后执行++操作，使其指向原来的元素，所以新插入的元素使顺序的
	for(auto i:l2){
		cout<<i<<" ";//4 3 2 1
	}
	cout<<endl;
	for(auto i:l3){
		cout<<i<<" ";//1 2 3 4
	}
	cout<<endl;
}
//istream迭代器
void test2(){
	/*
	istream_iterator<int> int_it(cin);
	istream_iterator<int> int_eof;//默认初始化为尾后迭代器
	ifstream in("a.txt");
	istream_iterator<string> str_it(in);//从a.txt中读取字符串
	*/
	/*
	vector<int> v;
	istream_iterator<int> in_it(cin),eof;
	while(in_it!=eof){//当未读到文件尾时
		v.push_back(*in_it++);
	}
	for(auto i:v){
		cout<<i<<" ";
	}
	cout<<endl;
	*/
	/*
	istream_iterator<int> in_it(cin),eof;
	vector<int> v(in_it,eof);//利用istream迭代器来构造v
	for(auto i:v){
		cout<<i<<" ";
	}
	cout<<endl;
	*/
	istream_iterator<int> in_it(cin),eof;
	cout<<accumulate(in_it,eof,0)<<endl;//通过istream迭代器从流读取数据并求和
}
//ostream迭代器
void test3(){
	vector<int> v={1,2,3,4};
	ostream_iterator<int> out_it(cout," ");//第二个是可选参数，是一个c风格字符串，每次输出后都将打印这个字符串
	for(auto i:v){
		*out_it++=i;//赋值语句把元素写到cout
		//out_it=i;//等价的操作
	}
	cout<<endl;
	copy(v.begin(),v.end(),out_it);//直接把值拷贝到cout中，省略循环
}
//反向迭代器
void test4(){
	vector<int> v={123,25,38,685};
	for(auto r_it=v.crbegin();r_it!=v.crend();++r_it){
		cout<<*r_it<<" ";//逆序输出
	}
	cout<<endl;
	sort(v.begin(),v.end());
	for(auto i:v){
		cout<<i<<" ";//从小到大排序
	}
	cout<<endl;
	sort(v.rbegin(),v.rend());
	for(auto i:v){
		cout<<i<<" ";//从大到小排序
	}
	cout<<endl;
}
//反向迭代器和其他迭代器的关系
void test5(){
	string line="first,middle,last";
	auto comma=find(line.cbegin(),line.cend(),',');
	cout<<string(line.cbegin(),comma)<<endl;//first
	auto rcomma=find(line.crbegin(),line.crend(),',');
	cout<<string(line.crbegin(),rcomma)<<endl;//tsal，因为反向迭代器会反向处理string，所以会反向打印
	cout<<string(rcomma.base(),line.cend())<<endl;//last，使用反向迭代器的base成员将其转换为一个不同迭代器，从而正向输出
}

int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	test5();

}

