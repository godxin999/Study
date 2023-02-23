/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/23 10:49:16
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <map>
#include <set>
using namespace std;
//使用域作用符提取类型的成员
void test1(){
	set<string>::value_type v1;//string类型
	set<string>::key_type v2;//string类型
	map<string,int>::value_type v3;//pair<const string,int>类型
	map<string,int>::key_type v4;//string类型
	map<string,int>::mapped_type v5;//int类型
}
//map的迭代器
void test2(){
	map<string,int> mp;
	mp.insert({"hello",1});
	auto it=mp.begin();
	cout<<it->first<<" "<<it->second;
	//it->first="new key";//关键字是const的，无法修改
	++it->second;//可以通过迭代器改变元素
}
//set的迭代器
void test3(){
	set<int> s={0,1,2,3,4,5,6,7,8,9};
	set<int>::iterator it=s.begin();
	//*it=1;//set的迭代器是const的
	cout<<*it<<" ";
	cout<<endl;
}
//关联容器遍历
void test4(){
	map<int,int> mp;
	for(int i=0;i!=10;++i){
		mp.insert({i,10-i});
	}
	auto it=mp.begin();
	while(it!=mp.end()){
		cout<<it->first<<" "<<it->second<<endl;
		++it;
	}
}
//向set中插入元素
void test5(){
	vector<int> v={2,4,6,8,2,4,6,8};
	set<int> s;
	s.insert(v.cbegin(),v.cend());//插入一个范围
	for(auto i=s.begin();i!=s.end();++i){
		cout<<*i<<" ";
	}
	cout<<endl;
	s.insert({1,3,5,7,1,3,5,7});//插入一个列表
	for(auto i=s.begin();i!=s.end();++i){
		cout<<*i<<" ";
	}
	cout<<endl;
}
//向map中添加元素
void test6(){
	map<string,int> mp;
	mp.insert({"word",1});
	mp.insert(make_pair("hello",2));
	mp.insert(pair<string,int> ("world",3));
	mp.insert(map<string,int>::value_type ("!",4));
	for(auto i=mp.begin();i!=mp.end();++i){
		cout<<i->first<<" "<<i->second<<endl;
	}
}
//非multi容器insert返回值
void test7(){
	map<int,int> mp;
	auto ret=mp.insert({100,200});
	//ret的实际类型为pair<map<string,int>::iterator,bool>
	cout<<ret.first->first<<" "<<ret.first->second<<" "<<ret.second<<endl;//insert返回一个pair，第一个关键字是迭代器，其指向具有指定关键字的元素，第二个关键字是一个bool值，如果插入成功则为1，反之为0
	ret=mp.insert({100,200});
	cout<<ret.first->first<<" "<<ret.first->second<<" "<<ret.second<<endl;
}
//erase函数
void test8(){
	set<int> s={1,2,3,4,5,6,7,8,9};
	auto ret=s.erase(0);
	cout<<ret<<endl;//0，即要删除的元素不在set里
	ret=s.erase(1);
	cout<<ret<<endl;//1，即删除元素的个数
	multiset<int> ms={1,1,1,1,1,2,3,4,5,6,7,8,9};
	ret=ms.erase(1);
	cout<<ret<<endl;//5，multiset中含有五个1
}
//map下标
void test9(){
	map<int,int> mp;
	mp[1]=1;//如果关键字并不存在，将会创建元素并插入到map中
	cout<<mp.size()<<endl;//1
	try{
		cout<<mp.at(2)<<endl;//使用at访问不存在的关键字将会抛出越界异常
	}
	catch(out_of_range err){
		cout<<err.what()<<endl;//invalid map<K, T> key
	}
	cout<<mp[1]<<endl;//1
	mp[1]++;//map下标操作返回左值，可以读也可以写
	cout<<mp[1]<<endl;//2
}
//查找元素
void test10(){
	set<int> s={0,1,2,3,4,5,6,7,8,9};
	cout<<(s.find(6)!=s.end())<<endl;//1
	cout<<(s.find(11)!=s.end())<<endl;//0
	cout<<s.count(1)<<endl;//1
	cout<<s.count(11)<<endl;//0
	multimap<int,int> m_mp;
	for(int i=0;i!=10;++i){
		m_mp.insert({i,i});
		m_mp.insert({i,10-i});
	}
	auto pos=m_mp.find(4);//查找元素所在位置
	if(pos!=m_mp.end()){//如果元素存在
		auto sum=m_mp.count(4);//获取数量
		while(sum--){
			cout<<pos->second<<endl;//因为相同关键字对应的值在multimap中相邻存储，所以可以通过递增迭代器来输出所有对应的值
			++pos;
		}
	}
}
//lower_bound和upper_bound
void test11(){
	multimap<int,int> m_mp;
	for(int i=0;i!=10;++i){
		m_mp.insert({i,i});
		m_mp.insert({i,10-i});
	}
	//lower_bound指向第一个匹配关键字的元素的位置，upper_bound指向最后一个匹配关键字的元素之后的位置，利用这两个成员函数可以确定出来一个范围
	for(auto i=m_mp.lower_bound(4);i!=m_mp.upper_bound(4);++i){
		cout<<i->second<<endl;
	}
}
//equal_range函数
void test12(){
	multimap<int,int> m_mp;
	for(int i=0;i!=10;++i){
		m_mp.insert({i,i});
		m_mp.insert({i,10-i});
	}
	//equal_range函数返回一个pair，其存储匹配关键字的元素的范围
	for(auto pos=m_mp.equal_range(4);pos.first!=pos.second;++pos.first){
		cout<<pos.first->second<<endl;
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
	//test11();
	test12();


}

