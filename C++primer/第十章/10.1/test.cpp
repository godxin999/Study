/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/20 10:00:59
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//find
void test1(){
	int a[]={1,2,3,4,5,6,7,8,9};
	int val=6;
	int *p=find(begin(a),end(a),val);
	cout<<*p<<endl;
	int *pp=find(a+1,a+7,val);
	cout<<*pp<<endl;
}

int main(){
	test1();
}

