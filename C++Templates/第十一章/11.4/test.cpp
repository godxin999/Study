/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/24 15:24:39
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>


//使用引用初始化模板会导致
//1.非模板参数的默认初始化失效
//2.不再能直接使用0来初始化非类型模板参数
//3.赋值运算符也不再可用，因为对于具有非static引用成员的类，其默认赋值运算符会被删除
template <typename T,T Z=T{}>
class Refmem{
private:
	T zero;
public:
	Refmem():zero(Z){}//调用默认构造函数的初始化为默认初始化
};
int null=0;
void test1(){
	//Refmem<int&> rm1;
	//Refmem<int&,0> rm2;
	//Refmem<int&,null> rm3,rm4;
	//rm3=rm4;
}
//decltype(auto)很容易得到引用类型，在模板参数中使用时，需要注意
//为了能保证模板参数被引用类型初始化后仍能正常使用赋值运算符，那么需要单独定义赋值运算符而不是使用默认生成的赋值运算符

int main(){
	test1();




	return 0;
}