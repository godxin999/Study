/*************************************************************************
	> File Name: item21.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/3 15:30:37
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <memory>
#include <vector>
//using namespace std;
//unique_ptr的基础实现
template <typename T,typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params){
	return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
//使用make_shared和make_unique可以保证异常安全
class widget{};
int func(){return 100;}
void prowidget(std::shared_ptr<widget> spw,int i){}
void test1(){
	prowidget(std::shared_ptr<widget>(new widget),func());//可能会导致内存泄漏，如果new分配完内存之后，func()函数抛出异常，shared_ptr的构造函数还没调用，这是就会发生内存泄漏
	prowidget(std::make_shared<widget>(),func());//异常安全
}
//使用make_shared和make_unique的开销更小
void test2(){
	std::shared_ptr<widget> spw1(new widget);//涉及两次内存分配，开销较大
	auto spw2=std::make_shared<widget>();//只需要一次内存分配
}
//只能使用new来定义自定义删除器的智能指针
void test3(){
	auto delwid=[](widget *pw){delete pw;};
	std::unique_ptr<widget,decltype(delwid)> upw(new widget,delwid);
	std::shared_ptr<widget> spw(new widget,delwid);
}
//make_shared和make_unique调用小括号进行构造
void test4(){
	auto upv1=std::make_unique<std::vector<int>>(10,20);
	std::cout<<upv1->size()<<std::endl;//10
	auto list={10,20};
	auto upv2=std::make_unique<std::vector<int>>(list);
	std::cout<<upv2->size()<<std::endl;//2
}
//使用new和make_shared创建的智能指针的声明周期不同
class BigType{};
void test5(){
	{
		std::weak_ptr<BigType> wp;
		{
			auto pBigType=std::make_shared<BigType>();
			auto sp1=pBigType;
			auto sp2=sp1;
			std::weak_ptr<BigType> wp2=sp1;
			wp=pBigType;//弱引用计数增加
		}
		//Bigtype的析构函数调用，但是内存没有被释放，因为弱引用计数不为0
	}
	//弱引用计数为0，内存被释放，因为make_shared创建的shared_ptr的对象和控制块是一起分配的，所以内存占用会比较久
	{
		std::weak_ptr<BigType> wp;
		{
			std::shared_ptr<BigType> pBigType(new BigType);
			auto sp1=pBigType;
			auto sp2=sp1;
			std::weak_ptr<BigType> wp2=sp1;
			wp=pBigType;//弱引用计数增加
		}
		//Bigtype的析构函数调用，内存被释放，但控制块内存未被释放
	}
	//弱引用计数为0，控制块内存被释放
}
//自定义删除器的智能指针保证异常安全
void test6(){
	auto del=[](widget *pw){delete pw;};
	std::shared_ptr<widget> spw(new widget,del);
	//prowidget(spw,func());//spw为左值，只能进行拷贝构造，开销较大
	prowidget(std::move(spw),func());//采用move将spw转化为右值，提高性能
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	test6();
}