/*************************************************************************
	> File Name: item22.h
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/7 21:54:24
 ************************************************************************/

#include <memory>
//成员中含有string、vector和gadget，如果头文件的实现改变了，那么就需要重新编译
/*
class widget{
public:
	widget();
private:
	std::string name;
	std::vector<double> data;
	gadget g1,g2,g3;
};
*/
//将数据成员替换成一个声明但未定义的类的指针，可以加速编译以及避免需要在头文件实现改变后重新编译
class widget{
public:
	widget();
	~widget();
private:
	struct Impl;
	Impl *pimpl;
};
//-----------------------------------
//使用unique_ptr
class widget{
public:
	widget();
	~widget();//先声明，在实现文件中进行定义
	widget(widget&&);//同理，移动会销毁pImpl指针指向的对象
	widget& operator=(widget&&);//同理，移动赋值会生成销毁pImpl的代码
	widget(const widget&);//不能使用default，因为可能存在move-only的类型
	widget& operator=(const widget&);//同上
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;//需要保证编译器生成销毁unique_ptr<Impl>的代码前Impl已经是一个完全类型
};