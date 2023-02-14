/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/14 19:15:14
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
//capacity和size
void test1(){
	vector<int> v;
	//没有元素，大小和容量均为0
	cout<<"v: size: "<<v.size()<<" capacity: "<<v.capacity()<<endl;//v: size: 0 capacity: 0
	for(auto i=0;i<24;i++){
		v.push_back(i);
	}
	//大小为24，容量由系统而定，大于等于24
	cout<<"v: size: "<<v.size()<<" capacity: "<<v.capacity()<<endl;//v: size: 24 capacity: 28
	v.reserve(50);//预分配额外空间
	cout<<"v: size: "<<v.size()<<" capacity: "<<v.capacity()<<endl;//v: size: 24 capacity: 50
	while(v.size()!=v.capacity()){//消耗所有预留空间
		v.push_back(0);
	}
	cout<<"v: size: "<<v.size()<<" capacity: "<<v.capacity()<<endl;//v: size: 50 capacity: 50
	v.push_back(1);//超出容量，额外进行分配
	cout<<"v: size: "<<v.size()<<" capacity: "<<v.capacity()<<endl;//v: size: 51 capacity: 75
	v.shrink_to_fit();//退还额外的多于内存，但是不保证能够退还
	cout<<"v: size: "<<v.size()<<" capacity: "<<v.capacity()<<endl;//v: size: 51 capacity: 51
}
int main(){
	test1();
}

