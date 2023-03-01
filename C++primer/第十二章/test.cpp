/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/24 10:28:50
 ************************************************************************/

#include "StrBlobPtr.h"
using namespace std;

int main(){
	StrBlob a = { "1","2","3","4","5"};
	StrBlobPtr p(a);
	++p;
	cout << p.deref() << endl;//2
	cout << p++.deref() << endl;//2
	cout << p.operator++(0).deref() << endl;//3，显式调用后缀递增
	cout << p.operator++().deref() << endl;//5，显式调用前缀递增
	*p = "100";
	cout << p->size() << endl;//p是定义了operator->的类，首先调用p.operator->，然后获得一个类型为string*的指针，然后调用指针内置的箭头运算符，进而获得size成员的值
	cout << (*p).size() << endl;//等价于p->size()
}

