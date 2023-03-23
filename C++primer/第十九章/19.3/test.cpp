/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/23 20:04:54
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//枚举成员
enum color {red,yellow,green};
//enum stoplight {red,yellow,green};//错误，重复定义枚举成员
enum class peppers{red,yellow,green};
void test1(){
	//peppers p=green;//错误，green不在有效的作用域中
	peppers p2=peppers::green;
}
//枚举值
enum class intTypes{
	//如果没有显式提供枚举值，则当前枚举成员的值为之前枚举成员的值加一，枚举值从0开始
	charType=8,shortType=16,intType=16,
	longType=32,longlongType=64
};
//枚举成员是const的，在初始化枚举成员时提供的初始值必须为常量表达式
void test2(){
	constexpr intTypes charBits=intTypes::charType;
}
//枚举类型的隐式转换
void test3(){
	int i=color::green;//不限定作用域的枚举类型的枚举成员隐式转换为int
	cout<<i<<endl;
	//int j=peppers::green;//限定作用域的枚举类型不会进行隐式转换
}
//枚举类型的前置声明，且指定enum的大小，默认情况下限定作用域的enum成员类型为int
enum intValues:unsigned long long;
enum class openMode;//默认为int
enum intValues:unsigned long long{//声明和定义必须匹配
	charType=255,shortType=65536,intType=65536,
	longType=4294967295UL,longlongType=18446744073709551615UL
};
//枚举类型和形参匹配
enum Tokens {INLINE=128,VIRTUAL=129};
void ff(Tokens){
	cout<<"ff(Tokens)"<<endl;
}
void ff(int){
	cout<<"ff(int)"<<endl;
}
void test4(){
	Tokens curTok=INLINE;
	ff(128);//ff(int)，即使整型值与枚举成员的值相同也不能作为函数的enum实参使用
	ff(INLINE);//ff(Tokens)
	ff(curTok);//ff(Tokens)
}
void newf(unsigned char){
	cout<<"newf(unsigned char)"<<endl;
}
void newf(int){
	cout<<"newf(int)"<<endl;
}
//不限定作用域的枚举类型的对象或者枚举成员传递给整型形参
void test5(){
	unsigned char uc=VIRTUAL;
	newf(VIRTUAL);//newf(int)，提升为int类型
	newf(uc);//newf(unsigned char)
}



int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	test5();

}

