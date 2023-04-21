/*************************************************************************
	> File Name: item15.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/21 22:26:11
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <array>
using namespace std;
//constexpr对象都是const，但是不是所有的const对象都是constexpr
void test1(){
	int sz;
	//constexpr auto arraySize1=sz;//错误，sz的值在编译器不可知
	//array<int,sz> data1;
	constexpr auto arraySize2=10;
	array<int,arraySize2> data2;
	const auto arraySize=sz;//正确，arraySize是sz的常量复制
	//array<int,arraySize> data;//错误，arraySize值在编译器不可知
}
//
/*
constexpr int pow(int base,int exp)noexcept{//C++11
	return (exp==0?1:base*pow(base,exp-1));
}
*/
constexpr int pow(int base,int exp)noexcept{//C++14
	auto result=1;
	for(int i=0;i<exp;++i){
		result*=base;
	}
	return result;
}
void test2(){
	constexpr auto numConds=5;
	array<int,pow(3,numConds)> results;
	cout<<results.size()<<endl;
}
//用户定义的字面值可以是constexpr
class Point{
public:
	constexpr Point(double xVal=0,double yVal=0)noexcept:x(xVal),y(yVal){}
	constexpr double xValue()const noexcept{
		return x;
	}
	constexpr double yValue()const noexcept{
		return y;
	}
	//C++14允许将void函数声明为constexpr，也允许constexpr函数修改对象的状态
	constexpr void setX(double newX)noexcept{//C++14
		x=newX;
	}
	constexpr void setY(double newY)noexcept{//C++14
		y=newY;
	}
private:
	double x,y;
};
constexpr Point midpoint(const Point& p1,const Point& p2)noexcept{
	return {(p1.xValue()+p2.xValue())/2,(p1.yValue()+p2.yValue())/2};
}
constexpr Point reflection(const Point& p)noexcept{
	Point result;
	result.setX(-p.xValue());
	result.setY(-p.yValue());
	return result;
}
void test3(){
	constexpr Point p1(9.4,27.7);//构造函数会在编译期运行
	constexpr Point p2(28.8,5.3);
	constexpr auto mid=midpoint(p1,p2);
	cout<<mid.xValue()<<endl<<mid.yValue()<<endl;//19.1 16.5
	constexpr auto ref=reflection(mid);
	cout<<ref.xValue()<<endl<<ref.yValue()<<endl;//-19.1 -16.5
}

int main(){
	//test1();
	//test2();
	test3();



}