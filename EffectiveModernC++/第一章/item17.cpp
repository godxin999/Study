/*************************************************************************
	> File Name: item17.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/25 20:06:58
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//当用户声明了移动操作，拷贝操作就是delete的
class widget{
public:
	widget(widget&&);
	widget& operator=(widget&&);
};
//当用户声明了析构函数时，拷贝操作不会自动生成
//当类中没有拷贝、移动和析构操作时，编译器会生成移动操作
class widget2{
public:
	~widget2(){}//不会产生移动和拷贝操作
	widget2(const widget2&)=default;
	widget2& operator=(const widget2&)=default;
};
//如果类需要具有移动性，就为移动操作加上=default
//声明移动性会抑制拷贝生成，如果拷贝性也需要支持，则为拷贝操作也加上=default
class base{
public:
	virtual ~base()=default;
	base(base&&)=default;
	base& operator=(base&&)=default;
	base(const base&)=default;
	base& operator=(const base&)=default;
};
//成员函数模板不会阻止拷贝操作和移动操作的生成
class widget3{
public:
	template <typename T>
	widget3(const T&);
	template <typename T>
	widget3& operator=(const T&);
};

int main(){

}