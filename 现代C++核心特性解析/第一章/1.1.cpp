/*************************************************************************
	> File Name: 1.1.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/2 22:29:47
 ************************************************************************/

#include <cstdio>
#include <limits>
#include <iostream>

//long long类型(至少64位)
void test1(){
	long long x1=65536<<16;//没有字面量后缀的情况下，65536被当作32位整型
	long long x2=65536LL<<16;//LL后缀表示long long类型
	std::cout<<x1<<" "<<x2<<std::endl;
}

//long long类型用于枚举和位域
enum class longlong_enum:long long{
	x1,x2
};

struct longlong_bitfield{
	long long x1:8;
	long long x2:24;
	long long x3:32;
};

void test2(){
	std::cout<<sizeof(longlong_enum::x1)<<std::endl;//8
	std::cout<<sizeof(longlong_bitfield)<<std::endl;//8
}

//long long的上下限
void test3(){
	std::cout<<std::numeric_limits<long long>::min()<<std::endl;//-9223372036854775808
	std::cout<<std::numeric_limits<long long>::max()<<std::endl;//9223372036854775807 
	std::cout<<std::numeric_limits<unsigned long long>::max()<<std::endl;//18446744073709551615
	//long long使用std::printf打印时使用%lld，无符号则使用%llu
}

int main(){
	//test1();
	//test2();
	test3();





}