/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/14 10:36:28
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <string_view>
#include <cassert>

//SL.str.1 使用std::string来拥有字符序列(避免使用C风格字符串)
//SL.str.2 使用std::string_view来指向字符序列(string_view不会进行内存分配)
void* operator new(std::size_t count){
	std::cout<<" "<<count<<" bytes"<<"\n";
	return malloc(count);
}
void getString(const std::string& str){

}
void getStringView(std::string_view str){

}
void test1(){
	std::cout<<"\n";
	std::cout<<"std::string"<<"\n";
	std::string large="0123456789-123456789-123456789-123456789-123456789";
	std::string substr=large.substr(10);
	std::cout<<"\n";

	std::cout<<"std::string_view"<<"\n";
	std::string_view largeStringView{large.c_str(),large.size()};
	largeStringView.remove_prefix(10);

	assert(substr==largeStringView);

	std::cout<<"\n";
	std::cout<<"getString"<<"\n";
	getString(large);
	getString("0123456789-123456789-123456789-123456789-123456789");
	const char message[]="0123456789-123456789-123456789-123456789-123456789";
	getString(message);
	std::cout<<"\n";

	std::cout<<"getStringView"<<"\n";
	getStringView(large);
	getStringView("0123456789-123456789-123456789-123456789-123456789");
	getStringView(message);
	std::cout<<"\n";
}

//SL.str.4 用char*来指向单个字符
//SL.str.5 使用std::byte来指向未必表示字符的字节值
//可以使用std::to_integer(std::byte b)将std::byte转换为int
//也可以使用std::byte{integer}进行反向转换，但是要求ineger是一个小于std::numeric_limits<unsigned char>::max()的非负值
//SL.str.12 对当作标准库string使用的字符串字面量使用后缀s
void test2(){
	std::string hello="hello";
	auto firstPair=std::make_pair(hello,5);
	//auto secondPair=std::make_pair("hello",15);//auto推断为const char*
	using namespace std::string_literals;
	auto secondPair=std::make_pair("hello"s,15);//auto推断为std::string
	if(firstPair<secondPair){
		std::cout<<"firstPair<secondPair"<<"\n";
	}
}

int main(){
	//test1();
	test2();




	return 0;
}