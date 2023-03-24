/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/23 21:45:47
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <functional>
#include <vector>
using namespace std;
//类成员指针
class Screen{
public:
	static const string Screen::*data(){
		return &Screen::contents;
	}
	char get_cursor()const {
		cout<<"get_cursor()const"<<endl;
		return contents[cursor];
	}
	char get()const{
		cout<<"get()const"<<endl;
		return 0;
	}
	char get(int i,int j)const{
		cout<<"get(int i,int j)const"<<endl;
		return 0;
	}
	Screen& home(){
		cout<<"home"<<endl;
		return *this;
	}
	Screen& forward(){
		cout<<"forward"<<endl;
		return *this;
	}
	Screen& back(){
		cout<<"back"<<endl;
		return *this;
	}
	Screen& up(){
		cout<<"up"<<endl;
		return *this;
	}
	Screen& down(){
		cout<<"down"<<endl;
		return *this;
	}
	using Action=Screen& (Screen::*)();//Action可以指向上面五个函数
	enum Directions{HOME,FORWARD,BACK,UP,DOWN};//设置枚举类型
	Screen& move(Directions cm){
		return (this->*Menu[cm])();//根据枚举值获取指向对应函数的指针，解引用后调用该函数
	}
private:
	string contents="hello";
	int cursor=0;
	static Action Menu[];//成员指针函数表
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
//成员函数指针
void test2(){
	auto pmf=&Screen::get_cursor;
	//char为返回类型，Screen::*pmf2为指向成员函数的指针，(int,int)为参数列表
	char (Screen::*pmf2)(int,int)const;//Screen::*两端的()必不可少，不然会被认为是一个无效的函数声明，
	pmf2=&Screen::get;
	//pmf2=Screen::get;//错误，成员函数和指向该成员的指针之间不存在自动转换规则
	Screen myScreen,*pScreen=&myScreen;
	//因为调用运算符的优先级更高，所以(pScreen->*pmf)和(myScreen.*pmf2)两端的括号不可缺少
	char c1=(pScreen->*pmf)();//get_cursor()const
	char c2=(myScreen.*pmf2)(0,0);//get(int i,int j)const，pmf2解引用后变成一个函数，向其的参数列表中传入参数即可调用该函数
}
//使用成员指针的类型别名
using Action=char (Screen::*)(int,int)const;
Screen& action(Screen &s,Action=&Screen::get){return s;}
void test3(){
	Action get=&Screen::get;//get指向Screen的get成员
	Screen myScreen;
	action(myScreen);//使用默认实参
	action(myScreen,get);//传入我们定义的get
	action(myScreen,&Screen::get);//显式传入地址
}
//成员指针函数表
Screen::Action Screen::Menu[]={//定义并初始化函数表
	&Screen::home,
	&Screen::forward,
	&Screen::back,
	&Screen::up,
	&Screen::down,
};
void test4(){
	Screen myScreen;
	myScreen.move(Screen::BACK);//back
	myScreen.move(Screen::HOME);//home
}
//将成员函数用作可调用对象
void test5(){
	vector<string*> pvec(10);
	function<bool (const string*)> fp=&string::empty;//利用function生成可调用对象
	auto it=find_if(pvec.begin(),pvec.end(),fp);
}
//使用mem_fn生成一个可调用对象
void test6(){
	vector<string> svec(10);
	auto it=find_if(svec.begin(),svec.end(),mem_fn(&string::empty));//mem_fn可以根据成员指针的类型自动推断可调用对象的类型
	auto f=mem_fn(&string::empty);
	f(*svec.begin());//传入一个string对象，f使用.*运算符调用empty()
	f(&svec[0]);//传入一个string*对象，f使用->*运算符调用empty()
}
//使用bind生成一个可调用对象
void test7(){
	vector<string> svec(10);
	auto it=find_if(svec.begin(),svec.end(),bind(&string::empty,std::placeholders::_1));//选中范围中的每个string，并将其绑定到新的可调用对象的第一个隐式实参上
	auto f=bind(&string::empty,std::placeholders::_1);
	f(*svec.begin());//f使用.*运算符调用empty()
	f(&svec[0]);//f使用->*运算符调用empty()
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	test7();
}

