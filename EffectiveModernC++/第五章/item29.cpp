/*************************************************************************
	> File Name: item29.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/15 11:41:23
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <array>
#include <chrono>
using namespace std;
//移动操作性能不一定很高
class widget{};
void test1(){
	vector<widget> vw(10000);//内存分配在堆上
	auto vw2=std::move(vw);//只需要移动一个指针
}
void test2(){
	array<widget,10000> aw;//内存分配在栈上
	auto aw2=std::move(aw);//每个对象都要进行移动，无论是拷贝还是移动都将花费线性时间
}
//std::string的移动操作
void test3(){
	string a="12345";//短字符串(小于15个字符)会被存储到string的缓冲区中，移动这种字符串并不比拷贝操作更快
	string b="12345678900987654321";//长字符串存储在堆中
}
//几种移动操作无优势的情况
//1.类没有提供移动操作
//2.类的移动操作不比拷贝操作效率高
//3.移动操作没有声明为noexcept，无法保证强异常安全
//4.源对象是一个左值(除极少数情况)
int main(){
	//test1();
	test2();
}