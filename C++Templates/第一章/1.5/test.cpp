/*************************************************************************
        > File Name: test.cpp
        > Author: godxin999
        > Mail: A996570107@163.com
        > Created Time: 2023/8/9 17:02:24
 ************************************************************************/

#include <algorithm>
#include <cstring>
#include <string>
#include <iostream>
#include <type_traits>

// 函数模板的重载
int max(int a, int b) {
  std::cout << "max(int a, int b)" << std::endl;
  return b > a ? b : a;
}
template <typename T> 
T max(T a, T b) {
  std::cout << "max(T a, T b)" << std::endl;
  return b > a ? b : a;
}
void test1() {
  ::max(7, 42);       // max(int a, int b)
  ::max<>(7.0, 42.0); // max(T a, T b)
  ::max('a', 'b');    // max(T a, T b)
  ::max<>(7, 42);     // max(T a, T b)，显式指定一个空的模板列表，表明其会被解析为一个模板调用
  ::max<double>(7, 42); // max(T a, T b)
  //::max('a', 42.7); // max(int a, int b)，'a'和42.7均被转换为int(warning)
}
// 显式指定返回类型
template <typename RT, typename T1, typename T2> 
RT max2(T1 a, T2 b) {
  return b > a ? b : a;
}
template <typename T1, typename T2> 
auto max2(T1 a, T2 b) {
  return b > a ? b : a;
}
void test2(){
	auto a=::max2(4,7.2);
	auto b=::max2<long double>(7.2,4);
	//auto c=::max2<int>(4,7.2);//两个模板都匹配，导致错误
}
//为C字符串和指针重载max
template <typename T>
T max3(T a, T b) {
  return b > a ? b : a;
}
template <typename T>
T* max3(T* a, T* b) {
  return *b > *a ? b : a;
}
const char* max3(const char* a, const char* b) {
  return std::strcmp(b, a) > 0 ? b : a;
}
void test3(){
	int a=7;
	int b=42;
	auto m1=::max3(a,b);
	std::cout<<m1<<std::endl;
	std::string s1="hey";
	std::string s2="you";
	auto m2=::max3(s1,s2);
	std::cout<<m2<<std::endl;
	int* p1=&b;
	int* p2=&a;
	auto m3=::max3(p1,p2);
	std::cout<<*m3<<std::endl;
	const char* x="hello";
	const char* y="world";
	auto m4=::max3(x,y);
	std::cout<<m4<<std::endl;
}
//按值传递可能导致悬空引用
template <typename T>
const T& max4(const T& a, const T& b) {
	std::cout<<"max4(const T& a, const T& b)"<<std::endl;
  return b > a ? b : a;
}

const char* max4(const char* a, const char* b) {//按值传递
	std::cout<<"max4(const char* a, const char* b)"<<std::endl;
  return std::strcmp(b, a) > 0 ? b : a;//返回一个局部对象的指针
}

template <typename T>
const T& max4(const T& a, const T& b, const T& c) {
  return max4(max4(a, b), c);//调用按值传递的const char*的max4时，max4(a,b)会返回一个局部对象，其生命期仅限于max4(a,b)，因为返回类型为引用，所以会导致返回局部对象的引用
}
void test4(){
	auto m1=::max4(7,42,68);
	std::cout<<m1<<std::endl;
	const char* s1="hello";
	const char* s2="world";
	const char* s3="you";
	auto m2=::max4(s1,s2,s3);
	std::cout<<m2<<std::endl;
}
//要确保函数模板在被调用时，其已经在前方某处定义
template <typename T>
T max5(T a, T b) {
	std::cout<<"max5(T a, T b)"<<" ";
  return b > a ? b : a;
}

template <typename T>
T max5(T a, T b, T c) {
	std::cout<<"max5(T a, T b, T c)"<<" ";
  return max5(max5(a, b), c);
}

int max5(int a, int b) {
	std::cout<<"max5(int a, int b)"<<" ";
  return b > a ? b : a;
}

void test5(){
	auto m1=::max5(7,42,68);
	//max5(T a, T b, T c) max5(int a, int b) max5(int a, int b) for clang
	//max5(T a, T b, T c) max5(T a, T b) max5(T a, T b) for g++
}

int main() {
	//test1();
	//test2();
	//test3();
	test4();
	//test5();

	return 0;
}
