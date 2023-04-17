/*************************************************************************
	> File Name: item12.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/17 17:26:58
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <memory>
using namespace std;
//在重写基类虚函数时，使用override关键字
class Base{
public:
	virtual void mf1()const{}
	virtual void mf2(int x){}
	virtual void mf3()&{}
	virtual void mf4()const{}
};
class Derived:public Base{
public:
	virtual void mf1()const override{
		cout<<"mf1() override"<<endl;
	}
	virtual void mf2(int x)override{
		cout<<"mf2() override"<<endl;
	}
	virtual void mf3()& override{
		cout<<"mf3() override"<<endl;
	}
	virtual void mf4()const override{
		cout<<"mf4() override"<<endl;
	}
};
void test1(){
	unique_ptr<Base> p=make_unique<Derived>();
	p->mf1();
	p->mf2(100);
	p->mf3();
	p->mf4();
}
//区别对待左值对象和右值对象
class widget{
public:
	using DataType=vector<double>;
	DataType& data()&{
		cout<<"&"<<endl;
		return values;
	}
	DataType data()&&{
		cout<<"&&"<<endl;
		return std::move(values);
	}
private:
	DataType values;
};
widget makewidget(){
	return widget();
}
void test2(){
	widget w;
	auto val1=w.data();//&
	auto val2=makewidget().data();//&&，makewidget函数返回右值，故使用移动构造val2
}
int main(){
	//test1();
	test2();
}