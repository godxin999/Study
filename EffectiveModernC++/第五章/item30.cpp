/*************************************************************************
	> File Name: item30.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/16 14:51:23
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
void f(const std::vector<int>& v){}
void f(std::size_t val){}
void f(int (*pf)(int)){}
//完美转发的失败情形
template <typename... Ts>
void fwd(Ts&&... param){
	f(std::forward<Ts>(param)...);
}
//1.初始化列表
void test1(){
	f({1,2,3});//正确，隐式转换为std::vector<int>
	//fwd({1,2,3});//错误，编译失败
	auto il={1,2,3};//需要定义一个局部变量然后再进行转发
	fwd(il);
}
//2.NULL和0都不能被完美转发为空指针
//3.类中的静态成员
class widget{
public:
	static const std::size_t MinVal=28;//常量表达式，仅是声明
};
void test2(){
	f(widget::MinVal);//正确，调用f(28)
	fwd(widget::MinVal);//错误，因为MinVal没有被定义，对其取地址时就会发生错误
}
const std::size_t widget::MinVal;//定义
void test3(){
	fwd(widget::MinVal);//正确，MinVal已经被定义
}
//4.函数重载和模板名
int processVal(int value){return 0;}
int processVal(int value,int priority){return 1;}
void test4(){
	f(processVal);
	//fwd(processVal);//错误，无法确定是哪个函数
}
template <typename T>
T workOnVal(T param){return param;}
void test5(){
	//fwd(workOnVal);//错误，无法确定
	using ProcessFuncType=int(*)(int);//使用别名明确类型
	ProcessFuncType processValPtr=processVal;
	fwd(processValPtr);
	fwd(static_cast<ProcessFuncType>(workOnVal));//使用static_cast转换为对应类型
}
//5.位域
struct IPv4Header{
	std::uint32_t version:4,
	IHL:4,
	DSCP:6,
	ECN:2,
	totalLength:16;
};
void test6(){
	IPv4Header h;
	f(h.totalLength);//正确，实参拷贝到形参
	//fwd(h.totalLength);//错误，无法对bit取地址(非常量引用不能绑定到位域)
	fwd(static_cast<std::uint32_t>(h.totalLength));//将值拷贝出来，然后再进行转发
}
void f2(const int& sz){}//只有常量引用才能绑定到位域
void test7(){
	IPv4Header h;
	f2(h.totalLength);//正确，编译器额外生成一个副本作为参数传入
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	test7();
}