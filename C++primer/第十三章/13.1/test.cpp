/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/26 17:11:21
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <memory>
#include <vector>
using namespace std;
//如果一个类需要自定义析构函数，那么其一定需要自定义拷贝赋值运算符和拷贝构造函数
class HasPtr{
public:
	HasPtr(const string &s=string()):ps(new string(s)),i(0){}
	~HasPtr(){
		delete ps;
	}
private:
	string *ps;
	int i;
};
HasPtr f(HasPtr hp){//HasPtr传递的是值，hp由拷贝得到
	HasPtr ret=hp;//拷贝hp
	return ret;//ret和hp被销毁，且因为析构函数，这块内存将会被delete两次
}
void test1(){
	HasPtr p("some values");
	f(p);//当f结束时p.ps指向的内存会被释放
	HasPtr q(p);//此时p、q都指向无效内存
}
//使用default来显式要求编译器生成合成的版本
class Sales_data {
public:
	Sales_data() = default;	
	Sales_data(const Sales_data&)=default;
	Sales_data& operator=(const Sales_data&);
	~Sales_data()=default;
private:
	double avg_price()const;
	std::string bookNo;
	unsigned units_sold = 0;
	double revenue=0.0;
};
Sales_data& Sales_data::operator=(const Sales_data&)=default;//定义在类外，不会被隐式声明成内联的
//使用delete禁用函数
struct Nocopy{
	Nocopy()=default;
	Nocopy(const Nocopy&)=delete;//=delete告诉编译器我们不能用任何方式使用它，=delete必须出现在函数第一次声明的位置
	Nocopy &operator=(const Nocopy&)=delete;//可以对任何函数使用=delete，而=default只能对编译器可以合成的默认拷贝函数和拷贝控制成员使用
	~Nocopy()=default;//析构函数不能是删除的，不然就无法定义该类型的变量，即使动态分配了这种类型的变量也不能被释放
	int num;
};
//private访问控制
class PrivateCpoy(){
public:
	PrivateCpoy()=default;
	~PrivateCpoy();//声明但不定义析构函数，既可以防止普通用户进行拷贝，也可以防止友元和友元函数进行拷贝
private://拷贝控制成员是private的，普通用户无法访问，但是为了阻止拷贝应该使用=delete，而不是将它们定义成private的
	PrivateCpoy(const PrivateCpoy&);
	PrivateCpoy &operator=(const PrivateCpoy&);
};



int main(){
	//test1();





}

