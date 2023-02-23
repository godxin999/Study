/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/23 9:59:54
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <set>
#include <map>
using namespace std;
//初始化关联容器
void test1(){
	set<int> st={1,2,3,4};
	map<int,int> mp={{1,2},{3,4},{5,6}};
	vector<int> v;
	auto it=back_inserter(v);
	for(auto i=0;i!=10;++i){
		*it=i;
		*it=i;
	}
	set<int> s(v.cbegin(),v.cend());
	multiset<int> ms(v.cbegin(),v.cend());
	cout<<v.size()<<endl;//20
	cout<<s.size()<<endl;//10
	cout<<ms.size()<<endl;//20
}
//自定义关联容器比较函数
bool cmp(const int &a,const int &b){
	return a>b;
}
void test2(){
	multiset<int,decltype(cmp)*> ms(cmp);
	for(int i=0;i!=10;++i){
		ms.insert(i);
	}
	ostream_iterator<int> out_it(cout," ");
	copy(ms.cbegin(),ms.cend(),out_it);
}
//pair对象函数
pair<int,int> f(int a){
	if(a&1)return {1,1};//采用列表初始化
	else return pair<int,int> ();//隐式构造返回值
}
void test3(){
	vector<pair<int,int> > v;
	for(int i=0;i!=10;++i){
		v.push_back(f(i));
	}
	cout<<v.size()<<endl;
	for(auto [i,j]:v){
		cout<<i<<" "<<j<<endl;
	}
}

int main(){
	//test1();
	//test2();
	test3();
}

