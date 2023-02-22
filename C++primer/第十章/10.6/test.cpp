/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/22 19:49:43
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <list>
using namespace std;

void test1(){
	list<int> l1={1,8,2,6},c_l1(l1);
	list<int> l2={5,3,4,7},c_l2(l2);
	l1.splice(l1.begin(),l2);
	for(auto i:l1){
		cout<<i<<" ";//5 3 4 7 1 8 2 6 
	}
	cout<<endl;
	cout<<"l2 has "<<l2.size()<<" element."<<endl;//l2 has 0 element.
	c_l1.merge(c_l2);//按顺序插入但并非排序，将c_l2中的元素插入到比其大的元素前
	for(auto i:c_l1){
		cout<<i<<" ";//1 5 3 4 7 8 2 6
	}
	cout<<endl;
}


int main(){
	test1();
}

