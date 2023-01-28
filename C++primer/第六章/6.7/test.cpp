/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/1/28 17:54:48
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//函数指针的使用
bool cmplen(const string &a,const string &b){return a.size()<b.size();}
bool (*pf)(const string&,const string&);//这里是声明一个函数指针，返回类型要与函数的返回类型一致，形参列表也要相同，语句末尾要有分号
void test1(){
	pf=cmplen;//等价于pf=&cmplen
	printf("%p\n",&cmplen);
	printf("%p\n",pf);
}
//使用函数指针的时候可以不用解引用
void test2(){
	pf=&cmplen;
	bool b1=pf("apple","banana");//1
	bool b2=(*pf)("hello","world");//0
	cout<<b1<<" "<<b2<<endl;
}
//函数指针被赋值为nullptr或者值为0的常量表达式，表示未指向任何函数
void test3(){
	pf=nullptr;
	pf=0;
}
//指向重载函数的指针服从函数重载的规则
void ff(int*){}
void ff(unsigned){}
void test4(){
	void (*pf2)(int*)=ff;
	void (*pf3)(unsigned)=ff;
	printf("%p\n",pf2);
	printf("%p\n",pf3);
}
//第三个参数未函数类型，将会自动转换为指向函数的指针
void use(const string &s1,const string &s2,bool pf(const string&,const string&)){
	cout<<"use"<<endl;
}
//等价声明
void use(const string &s1,const string &s2,bool (*pf)(const string&,const string&));
void test5(){
	use("a","b",cmplen);//可以把函数当作实参来使用，会自动转换为指针
}
//func和func2均为函数类型
typedef bool func(const string&,const string&);
typedef decltype(cmplen) func2;//decltype返回函数类型
//funcp和funcp2均为函数指针
typedef bool (*funcp)(const string&,const string&);
typedef decltype(cmplen) *funcp2;//decltype返回函数类型，加上*为函数指针
//use的等价声明
void use(const string&,const string&,func);
void use(const string&,const string&,funcp2);
//返回函数指针
//1.使用类型别名
using f=int(int*,int);//f为函数类型
using fp=int(*)(int*,int);//fp为函数指针
fp f1(int);//f1接受int作为形参，返回值是一个函数指针
f *f1(int);//显式指定返回类型为函数指针
//2.直接声明
int (*f1(int))(int*,int);
//3.尾置返回类型
auto f1(int)->int(*)(int*,int);
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();

}

