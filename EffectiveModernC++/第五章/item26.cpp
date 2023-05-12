/*************************************************************************
	> File Name: item26.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/12 22:47:40
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <type_traits>
#include <vector>
using namespace std;
vector<string> vs;
//不要对万能引用进行重载
template <typename T>
void func(T&& param){
	vs.emplace_back(std::forward<T>(param));
}
string getfromindex(int index){
	return string();
}
void func(int x){
	vs.emplace_back(getfromindex(x));
}
void test1(){
	func(22);//调用特化版本
	short x=25;
	//func(x);//错误，调用通用引用的func，导致将short类型emplace_back到vector<string>中，最终发生错误
}
class person{
public:
	template <typename T>//模板函数不影响构造函数的生成
	explicit person(T&& n):name(std::forward<T>(n)){}
	explicit person(int idx):name(getfromindex(idx)){}
	//person(const person& rhs);//编译器生成
	//person(person&& rhs);//编译器生成
private:
	string name;
};
void test2(){
	person p("Nancy");
	//auto p2(p);//错误，因为没有const，所以模板函数匹配度更高，n的类型推导为person&，无法用该类型初始化name
	const person p3("Nancy");
	auto p4(p3);//正确，调用编译器生成的构造函数，虽然模板也可以正确实例化，但是模板的优先级较编译器生成的构造函数低
}
class specialperson:public person{
public:
	//specialperson(const specialperson& rhs):person(rhs){}//错误，调用了父类的完美转发构造函数
	//specialperson(specialperson&& rhs):person(std::move(rhs)){}////错误，调用了父类的完美转发构造函数
};

int main(){
	//test1();
	test2();
}