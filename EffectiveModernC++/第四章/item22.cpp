/*************************************************************************
	> File Name: item22.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/7 21:54:27
 ************************************************************************/

#include <widget.h>
#include <string>
#include <vector>
//将内存分配和回收均写在实现代码中
struct widget::Impl{
	std::string name;
	std::vector<double> data;
	//gadget g1,g2,g3;
};

widget::widget():pImpl(new Impl){}
widget::~widget(){delete pImlp;}
//--------------------------------
struct widget::Impl{
	std::string name;
	std::vector<double> data;
	//gadget g1,g2,g3;
};

widget::widget():pImpl(std::make_unique<Impl>()){}
widget::~widget(){}

widget::widget(widget&& rhs)=default;
widget& widget::operator=(widget&& rhs)=default;

widget::widget(const widget& rhs):Impl(std::make_unique<Impl>(*rhs.pImpl)){}
widget& widget::operator=(const widget& rhs){
	*pImpl=*rhs.Impl;
	return *this;
}