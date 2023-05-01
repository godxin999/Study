/*************************************************************************
	> File Name: item19.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/1 19:35:05
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <memory>
using namespace std;
//自定义删除器
/*
class widget{
public:
	void process(vector<shared_ptr<widget>> &vsw){
		vsw.emplace_back(this);//emplace_back会原地构造一个shared_ptr<widget>，如果函数外已经存在了指向widget对象的指针，那么会导致未定义的行为
	}
};
*/
/*
class widget:public std::enable_shared_from_this<widget>{
public:
	void process(vector<shared_ptr<widget>> &vsw){
		vsw.emplace_back(shared_from_this());//shared_from_this再使用之前必须有一个shared_ptr指向该对象(已经存在一个关联的控制块)，否则会抛出异常
	}
};
*/
class widget:public std::enable_shared_from_this<widget>{
public:
	template <typename... Ts>
	static shared_ptr<widget> create(Ts&&... params);
	void process(vector<shared_ptr<widget>> &vsw){
		vsw.emplace_back(shared_from_this());
	}
private:
	//将ctors声明为私有的，并使客户端通过工厂方式创建shared_ptr
};
void test1(){
	auto del=[](widget *pw){
		//makelogentry(pw);
		delete pw;
	};
	unique_ptr<widget,decltype(del)> upw(new widget,del);//删除器类型是unique_ptr的一部分
	shared_ptr<widget> spw(new widget,del);//删除器类型不是shared_ptr的一部分
};
//如果两个shared_ptr具有不同的删除器，但是具有相同类型，可以存放到容器中进行管理
void test2(){
	auto del1=[](widget *pw){delete pw;};
	auto del2=[](widget *pw){delete pw;};
	shared_ptr<widget> pw1(new widget,del1);
	shared_ptr<widget> pw2(new widget,del2);
	vector<shared_ptr<widget>> vw;
	vw.push_back(pw1);
	vw.push_back(pw2);
	cout<<vw.size()<<endl;//2
}
//不要使用原始指针来构造shared_ptr，容易造成未定义的行为
void test3(){
	auto pw=new widget;
	shared_ptr<widget> spw1(pw);//为pw创建控制块
	shared_ptr<widget> spw2(pw);//为pw创建第二个控制块，多个控制块意味着多个引用计数，多个引用计数就意味着对象被销毁多次
}
//使用make_shared创建智能指针，如果必须传递给shared_ptr的构造函数原始指针，则使用直接new出来的指针
void test4(){
	auto del=[](widget *pw){delete pw;};
	shared_ptr<widget> spw1=make_shared<widget>();
	shared_ptr<widget> spw2(new widget,del);
	shared_ptr<widget> spw3(spw2);//spw2和spw3使用相同的控制块
}
//CRTP，奇异递归模板模式
template <class Derived>
struct base{
	void name(){
		static_cast<Derived*>(this)->impl();
	}
};
struct D1:public base<D1>{
	void impl(){
		cout<<"D1::impl()"<<endl;
	}
};
struct D2:public base<D2>{
	void impl(){
		cout<<"D2::impl()"<<endl;
	}
};
void test5(){
	D1 d1;
	d1.name();//D1::impl()
	D2 d2;
	d2.name();//D2::impl()
}


int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	test5();

}
