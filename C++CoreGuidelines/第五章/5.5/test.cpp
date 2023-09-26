/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/26 22:11:09
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//C.161 对于对称的运算符，应该使用非成员函数
class MyInt{
public:
	explicit MyInt(int i):val(i){}
	friend MyInt operator+(const MyInt& lhs, const MyInt& rhs){
		return MyInt(lhs.val + rhs.val);
	}
	friend MyInt operator+(const MyInt& lhs, int rhs){
		return MyInt(lhs.val + rhs);
	}
	friend MyInt operator+(int lhs, const MyInt& rhs){
		return MyInt(lhs + rhs.val);
	}
private:
	int val;
};
void test1(){
	MyInt a(1), b(2);
	MyInt c = a + b;
	auto d=a+5;
	auto e=5+a;
}
//C.164 避免隐式转换运算符，如operator bool()，可能会导致未知情况下的整型提升
//C.162/C.163 重载的操作应该大致等价，仅对大致等价的操作进行重载
void print(int a){}
void print(const std::string&){}
//C.168在操作数所在的命名空间定义重载运算符



int main(){
	test1();




	return 0;
}