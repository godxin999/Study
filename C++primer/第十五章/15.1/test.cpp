/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/2 17:56:09
 ************************************************************************/

#include "Bulk_quote.h"
using namespace std;

int main(){
	Quote a;
	Bulk_quote b;
	print_total(cout, a, 20);
	print_total(cout, b, 20);
	Quote item;
	Bulk_quote bulk;
	Quote* p = &item;
	p = &bulk;
	Quote& r = bulk;
	double undiscounted = p->Quote::net_price(42);//强行调用基类定义的函数的版本，而不管p的动态类型到底是什么，用来回避虚函数的机制

}

