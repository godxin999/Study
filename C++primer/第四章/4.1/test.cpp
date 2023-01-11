/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/1/11 11:27:33
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//使用decltype时左值和右值的不同
void test1(){
	int *p=nullptr;//p是int*类型
	cout<<is_same<decltype(*p),int&>::value<<endl;//解引用符生成左值，所以得到引用类型
	cout<<is_same<decltype(&p),int**>::value<<endl;//取地址符生成右值，所以得到指针类型，该指针指向int*即int**类型
}

int main(){
	//test1();
}
