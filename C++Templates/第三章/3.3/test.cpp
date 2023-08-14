/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/14 18:01:22
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//非类型模板参数的限制
//非类型模板参数通常只能为整形常量(枚举)，指向objects/functions/members的指针，objects/functions的左值引用，或者是std::nullptr_t
/*
template <double vat>
double pro(double v){
	return v*vat;
}
template <std::string name>
class myclass{};
*/
template <const char* name>
class myclass2{};
extern const char s1[]="hi";//外部链接
const char s2[]="hi";//内部链接
void test1(){
	//myclass2<"hello"> m;//错误，字符串字面值不能作为模板参数
	myclass2<s1> m1;//所有版本可行
	myclass2<s2> m2;//since C++11
	static const char s3[]="hi";//无链接属性
	myclass2<s3> m3;//since C++17
	
}
template <int I,bool B>
class C{};
void test2(){
	C<sizeof(int)+4,sizeof(int)==4> c1;//非类型模板参数可以是编译器表达式
	C<42,(sizeof(int)>4)> c;//如果使用了operator>则需要放在小括号内，防止阶段参数列表
}
int main(){
	test1();
	test2();
	
	return 0;
}