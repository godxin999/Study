/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/1/27 16:48:59
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//默认实参的使用
void s(int h=10,int w=20,char b='a'){
	cout<<"h = "<<h<<" "<<"w = "<<w<<" "<<"b = "<<b<<endl;
}
void test1(){
	s();//h = 10 w = 20 b = a
	s(1,2);//h = 1 w = 2 b = a
	s(1,2,'#');//h = 1 w = 2 b = #
}
//形参只能被赋予一次默认实参
void s2(int,int,char='a');
//void s2(int,int,char='*');//error: redefinition of default argument
void s2(int=1,int=2,char);//合法，保证后面所有的形参都已经被给与过默认形参

int a=1;
char ch='+';
int f(){return 1;}
void s3(int h=a,int w=f(),char c=ch){
	cout<<"h = "<<h<<" "<<"w = "<<w<<" "<<"c = "<<c<<endl;
}
void test2(){
	s3();//h = 1 w = 1 c = +
	ch='*';//改变了默认实参的值
	int a=100;//隐藏了外部作用域中的a，没有改变默认实参的值
	s3();//h = 1 w = 1 c = *
}
//constexpr不一定返回常量表达式
constexpr int nsz(){return 1;}//编译时会把对常量表达式的调用替换为其结果值，为了可以随时展开，constexpr函数被隐式地指定为内联函数
constexpr size_t scale(size_t cnt){return nsz()*cnt;}//实参为常量表达式时，返回值也是常量表达式，反之返回值不为常量表达式
int arr[scale(2)];//合法，scale(2)是常量表达式
int i=2;
//int a2[scale(i)];//报错，scale(i)不是常量表达式
//预处理宏的使用
void test3(){
	if(1){
		cerr<<"Error: "<<__FILE__
			<<" : in function "<<__func__
			<<" at line "<<__LINE__<<endl
			<<"        Complied on "<<__DATE__
			<<" at "<<__TIME__<<endl
			<<"        Msg is ... "<<endl;
	}
}
/*
Error: .\test.cpp : in function test3 at line 49
        Complied on Jan 27 2023 at 21:45:31
        Msg is ...
*/
int main(){
	//test1();
	//test2();
	test3();
}

