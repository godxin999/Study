/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/23 17:56:27
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;
//定义自定义类型的无序容器
struct item{
	int num;
};

size_t hasher(const item &i){
	return hash<int>() (i.num);//hash<key_type>返回hash值
}

bool cmp(const item &a,const item &b){//自定义==比较运算
	return a.num==b.num;
}

void test1(){
	using IU_multiset=unordered_multiset<item,decltype(hasher)*,decltype(cmp)*>;
	IU_multiset a(100,hasher,cmp);//第一个参数是桶的大小
	a.insert({1});
	cout<<a.size()<<endl;
}

int main(){
	test1();
}

