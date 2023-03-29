/*************************************************************************
	> File Name: item1.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/28 17:30:05
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <type_traits>
#include <array>
using namespace std;
/*
template <typename T>
void f(ParamType param);
f(expr);
*/
template <typename T>
void f(T &param){
	//cout<<boolalpha<<is_same<decltype(param),const char(&)[10]>::value<<endl;
	cout<<boolalpha<<is_same<decltype(param),void(&)(int,double)>::value<<endl;
}
//ParamType是一个指针或引用但不是通用引用，如果expr为一个引用，那么忽略引用部分
void test1(){
	int x=27;
	const int cx=x;
	const int &rx=cx;
	f(x);//T为int，param的类型为int&
	f(cx);//T为const int，param的类型为const int&
	f(rx);//T为const int，param的类型为const int&
}
//ParamType是一个通用引用，如果expr是左值，那么T和ParamType均会被推导为左值引用，如果为右值则满足之前的规则
template <typename T>
void f2(T &&param){}
void test2(){
	int x=27;
	const int cx=x;
	const int &rx=cx;
	f2(x);//x为左值，T和param类型均为int&
	f2(cx);//cx为左值，T和param类型均为const int&
	f2(rx);//rx为左值，T和param类型均为const int&
	f2(27);//27是右值，T为int，param的类型为int&&
}
//ParamType既不是指针也不是引用，如果expr的类型是一个引用，那么忽略这个引用，然后忽略顶层const和volatile，即通过值传递进行处理
template <typename T>
void f3(T param){
	//cout<<boolalpha<<is_same<decltype(param),const char*>::value<<endl;
	cout<<boolalpha<<is_same<decltype(param),void(*)(int,double)>::value<<endl;
}
void test3(){
	int x=27;
	const int cx=x;
	const int &rx=cx;
	f3(x);//T和param均为int
	f3(cx);//T和param均为int
	f3(rx);//T和param均为int

	const char* const ptr="Fun with pointers";
	f3(ptr);//T和param均为const char*，其顶层const被忽略
}
//数组实参
void test4(){
	const char name[]="Zhang Xin";
	const char *ptrtoName=name;//数组退化为指针

	f3(name);//数组实参会被视作指针实参，所以T会被推导为const char*
	f(name);//函数可以接受数组的引用，T就被推导为const char[10]，param被推导为const char(&)[10]
}
template <typename T,size_t N>//在编译器返回一个数组大小的常量值
constexpr size_t arraySize(T (&)[N])noexcept{
	return N;
}
void test5(){
	int a[]={1,3,5,7,9};
	cout<<arraySize(a)<<endl;//5
	array<int,arraySize(a)> array1;
}
//函数实参
void someFunc(int,double){}
void test6(){
	f3(someFunc);//param被推导为一个指向函数的指针，其类型为void(*)(int,double)
	f(someFunc);//param被推导为一个指向函数的引用，其类型为void(&)(int,double)
}
int main(){
	//test3();
	//test4();
	//test5();
	test6();
}