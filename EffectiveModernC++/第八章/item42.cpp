/*************************************************************************
	> File Name: item42.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/20 12:07:32
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <memory>
#include <regex>
using namespace std;
//emplacement函数避免产生临时对象
void test1(){
	vector<string> vs;
	vs.emplace_back("dsadsad");
	vs.emplace_back(10,'x');//直接转发给构造函数在vector对应内存位置构造一个string
}
//满足下列三种情况时emplacement函数性能更优
//1.传递的参数类型与容器的初始化类型不同
//2.容器不拒绝重复项为新值，比如在使用set时使用insert更优
//3.值通过构造添加到容器，而不是赋值
void test2(){
	vector<string> vs;
	vs.emplace(vs.begin(),"dadadas");//先调用移动赋值把所有的对象向后移，然后构造一个临时的string，然后再通过移动赋值到第一个元素的位置，这种情况emplace没有优势
}
//内存管理对象不适用emplacement函数
class widget{};
void killWidget(widget* pw){
	delete pw;
}
void test3(){
	vector<shared_ptr<widget>> ptrs;
	ptrs.push_back({new widget,killWidget});//创建临时变量，push_back抛出异常后临时对象被析构
	ptrs.emplace_back(new widget,killWidget);//emplace_back分配内存失败，抛出异常，原始指针发生了内存泄漏
	shared_ptr<widget> spw(new widget,killWidget);
	ptrs.push_back(std::move(spw));
}
//emplacement函数会调用显示构造函数，在使用时要注意传送正确的参数
void test4(){
	vector<regex> regexes;
	regexes.emplace_back(nullptr);//编译通过，但是错误
	//regexes.push_back(nullptr);//错误，regex构造函数被声明为explicit的
}
int main(){ 
	//test1();
	//test2();
	//test3();
	test4();



}