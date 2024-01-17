/*************************************************************************
	> File Name: 1.2.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/2 22:52:37
 ************************************************************************/

#include <iostream>
#include <string>

//utf-8、utf-16、utf-32
void test(){
	//char8_t是C++20新增的类型，用于表示utf-8编码的字符，使用C++17编译会发生错误
	char8_t str[]=u8"ab";//u8后缀表示utf-8字符串字面量
	char16_t str2[]=u"中国";//u后缀表示utf-16字符串字面量
	char32_t str3[]=U"中国";//U后缀表示utf-32字符串字面量
	//u16string、u32string是char16_t、char32_t的字符串类型，在C++11中引入
	std::string s1="中国";
	std::u8string s2=u8"12345";//汉字包含多个字符，不能使用u8string
	std::u16string s3=u"中国";
	std::u32string s4=U"中国";
	std::cout<<s1.size()<<" "<<s3.size()<<" "<<s4.size()<<std::endl;//6 2 2
}

//字符串连接规则
//如果两个字符串字面量具有相同的前缀，则连接后的字符串字面量也具有该前缀
//如果其中一个字符串字面量没有前缀，则将其视为与另一个字符串字面量具有相同前缀的字符串字面量
//其他的连接行为由具体实现者定义

//wchar_t
//wchar_t在windows的实现是16位的，而在linux的实现是32位的
//windows环境下我们可以使用L前缀表示宽字符字面量
//std::wstring是wchar_t的字符串类型


int main(){
	test();



	return 0;
}