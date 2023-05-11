/*************************************************************************
	> File Name: item25.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/11 10:16:35
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <chrono>
using namespace std;
//对右值引用使用std::move，对通用引用使用std::forward
class widget{
public:
	widget():name(string()){}
	widget(const widget&)=default;
	widget& operator=(const widget&)=default;
	widget(widget&& rhs):name(std::move(rhs.name)){}
	widget& operator=(widget&&)=default;
	template <typename T>
	void setName(T&& newName){
		name=std::forward<T>(newName);//如果形参是左值，就使用拷贝赋值，如果形参是右值就使用移动赋值
	}
	template <typename T>
	void setNameErr(T&& newName){
		name=std::move(newName);//错误，如果传入左值会把左值移动走
	}
	void setNameSep(const string& newName){
		name=newName;
	}
	void setNameSep(string&& newName){
		name=std::move(newName);
	}
private:
	string name;
};
void test1(){
	widget w;
	string n="abc";
	cout<<n<<endl;//"abc"
	w.setNameErr(n);//n被移动走后n变成了未定义的值
	cout<<n<<endl;//空
}
void test2(){
	widget w;
	w.setNameSep("Tom");//会先构造一个string临时变量
	w.setName("Tom");//可以避免临时变量的产生，直接进行移动赋值
}
//如果一个函数中多次使用绑定到通用引用和右值引用的对象，则需要确保在完成其他操作前这个对象不会被移动
template <typename T>
void setSignText(T&& text){
	sign.setText(text);//std::forward只在最后使用
	auto now=chrono::system_clock::now();
	signHistory.add(now,std::forward<T>(text));
}
//只有使用按值返回的函数且返回值绑定到右值引用或通用引用上时，需要对返回的引用使用std::forward或std::move
/*
Matrix operator+(Matrix&& lhs,const Matrix& rhs){
	lhs+=rhs;
	return std::move(lhs);//如果Matrix不支持移动操作，效率也不会变差
}
template <typename T>
Fraction reduceAndCopy(T&& frac){
	frac.reduce();
	return std::forward<T>(frac);
}
*/
//如果是按值返回的函数但返回值没有绑定到右值引用或通用引用上，则不要对返回值使用std::move
widget makewidget(){
	widget w;
	//return std::move(w);//返回局部对象的引用，影响编译器优化
	return w;//编译器自己进行返回值优化
}
int main(){
	//test1();
	test2();
}