/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/1/11 13:17:41
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

//一元正号作用于指针
void test1(){
	int a=100;
	int *p=&a;
	cout<<*(+p)<<endl;
}
//带符号取余运算
void test2(){
	//-m不溢出时,(-m)%n=-(m%n),m%(-n)=m%n
	int a=21;
	int b=6;
	cout<<a%b<<endl;
	cout<<(-a)%b<<endl;
	cout<<a%(-b)<<endl;	
	cout<<(-a)%(-b)<<endl;
	int m=-1<<31;//TMIN,-TMIN=TMIN
	int n=5;
	cout<<(-m)%n<<endl;//-3
	cout<<m%(-n)<<endl;//-3
}
int main(){
	//test1();
	test2();
}

