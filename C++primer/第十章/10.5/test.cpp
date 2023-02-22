/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/22 16:19:24
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;

void test1(){
	vector<int> v={0,1,2,3,4,4,4,4,5,5,5,5,6,7,8,9},v2;
	remove_copy_if(v.begin(),v.end(),back_inserter(v2),[](int i){//把偶数元素拷贝到v2中
		return i&1;
	});
	for(auto i:v2){
		cout<<i<<" ";//0 2 4 4 4 4 6 8 
	}
	cout<<endl;
	auto wc=remove_if(v.begin(),v.end(),[](int i){//移除奇数元素并返回剩余元素尾后位置的迭代器
		return i&1;
	});
	for(auto i:v){
		cout<<i<<" ";//0 2 4 4 4 4 6 8 5 5 5 5 6 7 8 9
	}
	cout<<endl;
	v.erase(wc,v.end());//使用erase擦除后保证没有多余元素
	for(auto i:v){
		cout<<i<<" ";//0 2 4 4 4 4 6 8
	}
	cout<<endl;
}

int main(){
	test1();
}

