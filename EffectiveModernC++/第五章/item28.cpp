/*************************************************************************
	> File Name: item28.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/14 16:44:27
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <type_traits>
using namespace std;
//引用折叠
template <typename T>
void func(T&& param){
	if(is_same_v<widget,T>){
		cout<<"rvalue reference"<<endl;
	}
	else if(is_same_v<widget&,T>){//widget& &&折叠为widget&
		cout<<"lvalue reference"<<endl;
	}
}
class widget{};
widget widgetfactory(){return widget();}
void test1(){
	widget w;
	func(w);//lvalue reference，T推导为widget&
	func(widgetfactory());//rvalue reference，T推导为widget
	auto&& a=w;//a为widget&
	auto&& b=widgetfactory();//b为widget&&
}
//std::forward实现
template <typename T>
T&& forward(remove_reference_t<T>& param){
	return static_cast<T&&>(param);
}
//引用折叠在typedef和using别名中依旧生效
template <typename T>
class w{
public:
	typedef T&& RvalueRefToT;
};
void test2(){
	w<int&> w1;
	cout<<boolalpha<<is_same_v<w<int&>::RvalueRefToT,int&><<endl;//true
	cout<<is_same_v<w<int>::RvalueRefToT,int&&><<endl;//true
}
//引用折叠在decltype中依旧生效
void test3(){
	auto func=[](auto&& widget){
		::func(std::forward<decltype(widget)>(widget));
	};
	widget w;
	func(w);//lvalue reference
	func(widgetfactory());//rvalue reference
}
int main(){
	//test1();
	//test2();
	test3();
}