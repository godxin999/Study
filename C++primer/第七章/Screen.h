#pragma once
#include <iostream>
#include <cstring>
class Screen {
public:
	typedef std::string::size_type pos;//类型别名
	//using pos=std::string::size_type;
private:
	pos cursor = 0;//光标
	pos height = 0, width = 0;//宽高
	std::string contents;
	mutable size_t access_ctr = 0;//mutable关键字声明的数据成员是可变数据成员，任何成员函数都可以改变其值
public:
	Screen() = default;//采用默认构造函数，在类的内部，默认内联
	Screen(pos h, pos w, char c) :height(h), width(w), contents(h* w, c) {}
	char get()const { return contents[cursor]; }//读取光标处的字符，定义在类内，隐式内联
	inline char get(pos h, pos w)const;//显式内联，const表示只读
	Screen& move(pos r, pos c);//在定义时设为内联
	void some_member()const;//记录成员函数被调用的次数
	Screen &set(char c);//设置字符
	Screen &set(pos r, pos c, char ch);//在对应位置设置字符的重载函数
	Screen &display(std::ostream& os) { do_display(os); return *this; }//非常量对象会调用这个
	const Screen &display(std::ostream& os)const { do_display(os); return *this; }//常量对象会调用这个
private:
	void do_display(std::ostream& os)const { os << contents; }//打印
};