/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/2 17:56:09
 ************************************************************************/

#include "Bulk_quote.h"
using namespace std;
//虚函数
void test1() {
	Quote a;
	Bulk_quote b;
	print_total(cout, a, 20);
	print_total(cout, b, 20);
}
//动态绑定以及回避虚函数机制
void test2() {
	Quote item;
	Bulk_quote bulk;
	Quote* p = &item;
	p = &bulk;
	Quote& r = bulk;
	double undiscounted = p->Quote::net_price(42);//强行调用基类定义的函数的版本，而不管p的动态类型到底是什么，用来回避虚函数的机制
}
//使用成员由静态类型决定
void test3() {
	Bulk_quote bulk;
	Bulk_quote* bulkP = &bulk;//静态类型和动态类型一致
	Quote* itemP = &bulk;//静态类型和动态类型不一致
	bulkP->discount_policy();
	//itemP->discount_policy();//错误，itemP的类型为Quote*，其中不含有discount_policy成员
}
//虚析构函数进行动态绑定
void test4() {
	Quote* ip = new Quote;
	delete ip;//调用Quote的析构函数
	ip = new Bulk_quote;
	delete ip;//调用Bulk_quote的析构函数
}

int main(){
	//test1();
	//test2();
	//test3();
	test4();
}

