/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/22 10:11:39
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <format>
#include <source_location>

void test1(){
	auto s=std::format("hello {}","world");
	std::cout<<s<<std::endl;
	auto s2=std::format("hello {0} {1} {3} {2}",42,"ccc",'c',3.14159);
	std::cout<<s2<<std::endl;
	auto s3=std::format("hello {0} {1} {0}",42,3.14159);
	std::cout<<s3<<std::endl;
}
void test2(){
	auto s=std::format("hello {:4d}",42);//右对齐宽度4
	std::cout<<s<<std::endl;//hello   42
	auto s2=std::format("hello {:04d}",42);//右对齐宽度4，前面补0
	std::cout<<s2<<std::endl;//hello 0042
}
void test3(){
	auto s=std::format("[{:4x}]",42);//x按16进制打印
	std::cout<<s<<std::endl;//[  2a]
	auto s2=std::format("[{:4X}]",42);//X大写
	std::cout<<s2<<std::endl;//[  2A]
}
void test4(){
	auto s=std::format("[{:<4d}]",42);//<左对齐
	std::cout<<s<<std::endl;//[42  ]
	auto s2=std::format("[{:>4d}]",42);//>右对齐
	std::cout<<s2<<std::endl;//[  42]
	auto s3=std::format("[{:^4d}]",42);//^居中对齐
	std::cout<<s3<<std::endl;//[ 42 ]
}
void test5(){
	auto s=std::format("[{:+4d}]",42);//+表示正数也打印+号
	std::cout<<s<<std::endl;//[ +42]
	auto s2=std::format("[{:+4d}]",-42);
	std::cout<<s2<<std::endl;//[ -42]
	auto s3=std::format("[{:-4d}]",42);//-表示符号仅用于负数
	std::cout<<s3<<std::endl;//[  42]
}
void test6(){
	auto s=std::format("[{:#4x}]",42);//#表示打印前缀
	std::cout<<s<<std::endl;//[0x2a]
	auto s2=std::format("[{:#4o}]",42);
	std::cout<<s2<<std::endl;//[ 052]
	auto s3=std::format("[{:#4b}]",42);
	std::cout<<s3<<std::endl;//[0b101010]
	auto s4=std::format("[{:#4d}]",42);
	std::cout<<s4<<std::endl;//[  42]
}
void test7(){
	auto s=std::format("[{:.4f}]",3.14);//.4保留四位小数
	std::cout<<s<<std::endl;//[3.1400]
	auto s2=std::format("[{:.4f}]",13.14);
	std::cout<<s2<<std::endl;//[13.1400]
	auto s3=std::format("[{:X<10.4f}]",13.14);//宽度为10，左对齐，不足用X填充，保留四位小数
	std::cout<<s3<<std::endl;//[13.1400XXX]
	auto s4=std::format("[{:X>10.4f}]",13.14);//宽度为10，右对齐，不足用X填充，保留四位小数
	std::cout<<s4<<std::endl;//[XXX13.1400]
	auto s5=std::format("[{:X>+10.4f}]",13.14);//+表示正数也打印+号
	std::cout<<s5<<std::endl;//[XX+13.1400]
	auto s6=std::format("[{: >+15.3e}]",3.14);//e表示使用科学计数法
	std::cout<<s6<<std::endl;//[     +3.140e+00]
}
void test8(){
	int n=5;
	auto s=std::format("[{: >+10.{}f}]",13.14,n);
	std::cout<<s<<std::endl;
}
void test9(){
	auto s=std::format("[{:>20.5s}]","abcdefg");//s表示字符串，宽度20，保留5个字符
	std::cout<<s<<std::endl;//[               abcde]
	auto s2=std::format("[{:^20s}]","abcdefg");
	std::cout<<s2<<std::endl;//[      abcdefg       ]
}
void test10(){
	auto sl=std::source_location::current();
	std::cout<<std::format("line = {}, column = {}, file = {}, func = {}",sl.line(),sl.column(),sl.file_name(),sl.function_name())<<std::endl;
}


int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7();
	//test8();
	//test9();
	test10();




	return 0;
}
