/*************************************************************************
	> File Name: 13.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/15 15:56:05
 ************************************************************************/

#include <iostream>
//继承构造函数
class Base{
public:
	Base(){}
	Base(int x):x(x),y(0){}
	Base(int x, int y):x(x),y(y){}
private:
	int x;
	int y;
};
class Derived: public Base{
	using Base::Base;
};
//1.派生类隐式继承基类的构造函数，只有程序中使用了这些构造函数，编译器才会生成相应的代码
//2.派生类不会继承基类的默认构造函数和拷贝构造函数
//3.继承构造函数不会影响派生类默认构造函数的隐式声明
//4.在派生类声明签名相同的构造函数会禁止继承相应的构造函数
//5.派生类继承多个签名相同的构造函数，会导致二义性
//6.继承构造函数的基类构造函数不能为私有的


int main(){





	return 0;
}