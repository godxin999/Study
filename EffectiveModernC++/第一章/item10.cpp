/*************************************************************************
	> File Name: item10.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/7 23:20:59
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <tuple>
#include <type_traits>
using namespace std;
//非限域枚举
/*
enum Color{black,white,red};
auto white=false;//错误，white已经在这个作用域了
*/
//限域枚举
enum class Color{black,white,red};
auto white=false;
//Color c1=white;//错误，white不在该作用域
Color c2=Color::white;//正确，加上对应的作用域
//非限域枚举可能会发生隐式转换
void test1(){
	enum Color{black,white,red};
	Color c=red;
	if(c<14.5){
		cout<<"Implicit conversion"<<endl;
	}
}
//在限域枚举中可以用cast进行类型转换
void test2(){
	enum class Color{black,white,red};
	Color c=Color::red;
	if(static_cast<double>(c)<14.5){
		cout<<"static_cast<double>"<<endl;
	}
}
//枚举可以进行前置声明，非限域枚举需要指定类型
enum class A;//默认基础类型为int
enum B : unsigned;
//非限域枚举在某种情况下是很有用的
using UserInfo=tuple<string,string,size_t>;
void test3(){
	UserInfo uInfo;
	auto val=get<1>(uInfo);//在实际使用时可能无法记住1为什么
	enum UserInfoFields{uiName,uiEmail,uiReputation};
	auto val2=get<uiName>(uInfo);//枚举类型直接指示类型，此处获取用户名字
}
void test4(){
	UserInfo uInfo;
	enum class UserInfoFields{uiName,uiEmail,uiReputation};
	auto val=get<static_cast<size_t>(UserInfoFields::uiName)>(uInfo);
}
//使用constexpr模板函数在编译期接受任意枚举值并返回其值
template <typename E>
constexpr typename std::underlying_type<E>::type toUType(E enumrator)noexcept{
	return static_cast<typename std::underlying_type<E>::type>(enumrator);
}
template <typename E>
constexpr auto toUType2(E enumrator)noexcept{
	return static_cast<std::underlying_type_t<E>>(enumrator);
}
void test5(){
	UserInfo uInfo;
	enum class UserInfoFields{uiName,uiEmail,uiReputation};
	auto val1=get<toUType2(UserInfoFields::uiEmail)>(uInfo);
	auto val2=get<toUType2(UserInfoFields::uiName)>(uInfo);
	auto val3=get<toUType2(UserInfoFields::uiReputation)>(uInfo);
	cout<<val1<<" "<<val2<<" "<<val3<<endl;
}

int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	test5();
}