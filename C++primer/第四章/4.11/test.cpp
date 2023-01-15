/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/1/15 19:16:15
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//整型提升
void test1(){
	char a=0xb6;
    short b=0xb600;
    int c=0xb6000000;
    if(a==0xb6)cout<<1<<endl;
    if(b==0xb600)cout<<2<<endl;
    if(c==0xb6000000)cout<<3<<endl;
	if(a==(char)0xb6)cout<<4<<endl;//想要判断必须显式转换为小整数类型
}
//数组转换为指针，数组作为decltype的参数或者作为取地址符(&)、sizeof的运算对象时不会发生转换
void test2(){
	int a[10]={0};
	decltype(a) c;//decltype返回数组类型，与a相同
	cout<<sizeof(c)<<endl;
}
//强制类型转换
void test3(){
	int i=1,j=2;
	double a=static_cast<double>(i)/j;//强制转换为浮点数
	cout<<a<<endl;
	void *p=&a;
	double *dp=static_cast<double*>(p);//将void*转换为原始类型
	const char *ccp="apple";
	char *cp=const_cast<char*>(ccp);//改变了常量属性，但是不可通过pp写值
	string s=static_cast<string>(ccp);//合法，把字符转字面值转换为string类型
	cout<<s<<endl;
	//const_cast<string>(pc);//报错，const_cast只能改变常量属性
	int num1=0x64636261;
	int *ip1=&num1;
	char *cp1=reinterpret_cast<char*>(ip1);//reinterpret_cast为运算对象的位模式提供较低层次上的重新解释
	cout<<hex<<*ip1<<endl;
	cout<<cp1<<endl;//字符串以apple后的\0结束
	int num2=0x00636261;
	int *ip2=&num2;
	char *cp2=reinterpret_cast<char*>(ip2);
	cout<<hex<<*ip2<<endl;
	cout<<cp2<<endl;//num2中带有0x00，所以只输出abc
}

int main(){
	//test1();
	//test2();
	test3();



}

