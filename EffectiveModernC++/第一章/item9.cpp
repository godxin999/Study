/*************************************************************************
	> File Name: item9.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/6 20:38:57
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <list>
#include <type_traits>
using namespace std;
//使用using别名声明而不是typedef
typedef void(*PF)(int,const string&);
using FP=void(*)(int,const string&);//使用using别名声明函数指针更加直观
//别名可以被模板化而typedef不能
template <typename T>
using MyAllocList=std::list<T,MyAlloc<T>>;//直接使用using别名声明
//MyAllocList<widget> lw;

template <typename T>
struct MyAllocList{
	typedef std::list<T,MyAlloc<T>> type;//需要在struct内嵌套typedef来实现
};
//MyAllocList<widget>::type lw;
//如果你要在一个模板内使用typedef声明的对象，而这个对象又使用了模板参数，那么就必须在typedef前加上typename，如果使用了using别名声明则不需要使用typename，同时省略了::type后缀
template <typename T>
class widget{
private:
	MyAllocList<T> list;//因为MyAllocList是一个别名模板，它一定是一个类型名，它不是一个依赖类型，所以不用加typename
};

template <typename T>
class widget2{
private:
	typename MyAllocList<T>::type list;//因为可能存在MyAllocList的一个特化版本中MyAllocList::type不是一个类型名称，所以MyAllocList::type的类型依赖于T，所以需要加上typename使type被当作一个类型
};
class wine{};
template<>
class MyAllocList<wine>{
private:
	enum class WineType{
		White,Red,Rose
	};
	WineType type;//在这个特例化的类中，type是一个数据成员
};
//type_traits中的例子，在C++11中使用struct内嵌套typedef的形式实现，在C++14中使用using别名声明实现
/*
std::remove_const<T>::type//C++11 const T -> T
std::remove_const_t<T>//C++14

std::remove_reference<T>::type//C++11 T&/T&& -> T
std::remove_reference_t<T>//C++14

std::add_lvalue_reference<T>::type//C++11 T->T&
std::add_lvalue_reference_t<T>//C++14 
*/
//在C++11中使用using进行仿写
template <class T>
using remove_const_t = typename remove_const<T>::type;
template <class T>
using remove_reference_t = typename remove_reference<T>::type;
template <class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
int main(){






}

