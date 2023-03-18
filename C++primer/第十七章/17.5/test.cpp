/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/17 20:42:47
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <cmath>
using namespace std;
//控制布尔值的格式
void test1(){
	cout<<"default bool values: "<<true<<" "<<false<<endl;//default bool values: 1 0
	cout<<"alpha bool values: "<<boolalpha<<true<<" "<<false<<endl;//alpha bool values: true false
	cout<<noboolalpha<<true<<" "<<false<<endl;//1 0，使用noboolalpha恢复为默认格式
}
//指定整型的进制
void test2(){
	cout<<"default: "<<20<<" "<<1024<<endl;//default: 20 1024
	cout<<"octal: "<<oct<<20<<" "<<1024<<endl;//octal: 24 2000
	cout<<"hex: "<<hex<<20<<" "<<1024<<endl;//hex: 14 400
	cout<<"decimal: "<<dec<<20<<" "<<1024<<endl;//decimal: 20 1024，这里将进制重新转换为10进制
}
//使用showbase在输出结果中显式进制
void test3(){
	cout<<showbase;
	cout<<"default: "<<20<<" "<<1024<<endl;//default: 20 1024
	cout<<"octal: "<<oct<<20<<" "<<1024<<endl;//octal: 024 02000
	cout<<"hex: "<<hex<<20<<" "<<1024<<endl;//hex: 0x14 0x400
	cout<<"decimal: "<<dec<<20<<" "<<1024<<endl;//decimal: 20 1024
	cout<<noshowbase;//恢复默认状态
	//使用upperbase在16进制中打印大写X
	cout<<uppercase<<showbase<<"hex: "<<hex<<20<<" "<<1024<<nouppercase<<noshowbase<<dec<<endl;//hex: 0X14 0X400
}
//控制浮点数格式
void test4(){
	//cout.precision()返回当前精度值，浮点数的精度默认为6位
	cout<<"Precision: "<<cout.precision()
		<<", Value: "<<sqrt(2.0)<<endl;//Precision: 6, Value: 1.41421
	cout.precision(12);//设置打印精度为12位
	cout<<"Precision: "<<cout.precision()
		<<", Value: "<<sqrt(2.0)<<endl;//Precision: 12, Value: 1.41421356237
	cout<<setprecision(3);//使用setprecision操纵符设置精度
	cout<<"Precision: "<<cout.precision()
		<<", Value: "<<sqrt(2.0)<<endl;//Precision: 3, Value: 1.41
}

int main(){
	//test1();
	//test2();
	//test3();
	test4();
}