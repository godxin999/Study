/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/2 15:33:08
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//类型转换运算符
class SmallInt{
public:
	SmallInt(int i=0):val(i){
		if(i<0||i>255){
			throw out_of_range("Bad SmallInt value");
		}
	}
	/*
	operator int()const{//定义类型转换运算符，支持把SmallInt类型转换为int类型
		//cout<<"SmallInt to int"<<endl;
		return val;
	}
	*/
	explicit operator int()const{
		return val;
	}
private:
	size_t val;
};
void test1(){
	SmallInt si;
	si=4;//首先把4隐式转换为SmallInt类型，然后调用SmallInt::operator=
	//si+3;//首先将si隐式转换为int，然后执行整数加法
	si=3.14;//调用参数为int的构造函数
	//si+3.14;//先转换为int，然后转换为double
}
//显式的类型转换运算符
void test2(){
	SmallInt si=3;
	//si+3;//错误，此处需要隐式的类型转换，而类型转换运算符被声明为显式的
	static_cast<int>(si)+3;//正确，显式请求类型转换
}

int main(){
	//test1();
	test2();




}

