/*************************************************************************
	> File Name: item27.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/13 21:48:23
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <vector>
using namespace std;
vector<string> vs;
string getfromindex(int index){
	return string();
}
//万能引用重载的替代方法
//1.放弃重载万能引用
//2.将通用引用替换为const T&
//3.按值传递
//4.使用标签分发技术
template <typename T>
void func(T&& param){
	//std::is_integral<T>判断T书否为int类型，但是其会把左值判断为int&
	//funcImpl(std::forward<T>(param),std::is_integral<T>());
	funcImpl(std::forward<T>(param),std::is_integral<std::remove_reference_t<T>>());//使用std::remove_reference_t<T>去除引用
}
void funcImpl(int x,std::true_type){
	vs.emplace_back(getfromindex(x));
}
template <typename T>
void funcImpl(T&& param,std::false_type){
	vs.emplace_back(std::forward<T>(param));
}
//使用约束的万能引用的模板
class person{
public:
	//std::enable_if_t约束模板,std::decay_t<T>对类型进行退化
	/*template <typename T,typename = typename std::enable_if_t<!std::is_same_v<person,std::decay_t<T>>>>*/
	template <typename T,typename = typename std::enable_if_t<!std::is_base_of_v<person,std::decay_t<T>>&&!std::is_integral_v<std::remove_reference_t<T>>>>//std::is_base_of_v判断后面的类是否是前面的类的子类或其本身
	explicit person(T&& n):name(std::forward<T>(n)){
		static_assert(std::is_constructible_v<std::string,T>,"Parameter n can't be used to construct a std::string");//使用静态断言发出指定的错误消息
	}
	explicit person(int idx):name(getfromindex(idx)){}
	//person(const person& rhs);
	//person(person&& rhs);
private:
	string name;
};
class specialperson:public person{
public:
	specialperson():person(""){}
	specialperson(const specialperson& rhs):person(rhs){}
	specialperson(specialperson&& rhs):person(std::move(rhs)){}
};
void test1(){
	person p("Nancy");
	auto p2(p);
}
void test2(){
	specialperson p;
	specialperson p1(p);
	auto p2(std::move(p1));
}
void test3(){
	//person p(u"Tom");//char16_t类型报错
}

int main(){
	//test1();
	//test2();
	test3();



}