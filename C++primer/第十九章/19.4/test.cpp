/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/23 21:45:47
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//类成员指针
class Screen{
public:
	static const string Screen::*data(){
		return &Screen::contents;
	}
private:
	string contents="hello";
};
void test1(){
	//const string Screen::*pdata1;//pdata1可以指向Screen对象的string成员
	//pdata1=&Screen::contents;
	//auto pdata2=&Screen::contents;//将取址运算符作用于Screen类的成员
	Screen myscreen,*pscreen=&myscreen;
	//auto s1=myscreen.*pdata1;//利用.*运算符解引用pdata1得到myscreen对象的contents成员
	//s=pscreen->*pdata1;//利用->*运算符解引用pdata1得到myscreen对象的contents成员
	const string Screen::*pdata3=Screen::data();//利用静态函数获得指向私有成员的指针
	auto s2=myscreen.*pdata3;//*解引用指针.获取对应的成员，进而获得myscreen的contents成员
	cout<<s2<<endl;
}


int main(){
	test1();





}

