/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/3 17:42:33
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//虚函数
struct B{
	virtual void f1(int)const;
	virtual void f2();
	void f3();
};
struct D1:B{
	void f1(int)const override;
	//void f2(int)override;//错误，B中没有形如f2(int)的函数
	//void f3()override;//错误，f3不是虚函数
	//void f4()override;//错误，B中没有名为f4()的函数
};
//final对函数声明
struct D2:B{
	void f1(int)const final;//不允许后续的其他类覆盖f1(int)
};
struct D3:D2{
	void f2();//正确，覆盖从间接基类B继承而来的f2
	//void f1(int)const;//错误，D2已经把f2声明为final
};

int main(){

}

