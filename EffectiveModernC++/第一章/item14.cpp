/*************************************************************************
	> File Name: item14.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/20 21:58:15
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
//函数不抛出异常的写法
int f1(int x)throw();//C++98，编译器进行较少的优化
int f2(int x)noexcept;//C++11，编译器极尽所能优化
struct widget{};
void test1(){
	vector<widget> vw;
	widget w;
	vw.push_back(w);//当移动构造被声明为noexcept时才可以进行移动，否则就会调用拷贝构造函数 
}
//带条件的异常声明
template <class T,size_t N>
void swap(T (&a)[N],T(&b)[N])noexcept(noexcept(swap(*a,*b)));
template <class T1,class T2>
struct pair{
	T1 first;
	T2 second;
	void swap(pair &p)noexcept(noexcept(swap(first,p.first))&&noexcept(noexcept(swap(second,p.second))));
};
//有些函数不声明noexcept但是也不会抛出异常，比如一些C语言写的库函数
void setup(){}
void cleanup(){}
void dowork()noexcept{//合法，编译器会认为该函数不会抛出异常
	setup();
	//...
	cleanup();
}
int main(){

}