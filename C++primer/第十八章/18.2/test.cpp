/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/20 13:13:34
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

//内联命名空间
namespace allver{
	//如果每个命名空间都定义在同名的头文件中，则可以使用include来进行定义
	//#include "v1.h"
	//#include "v2.h"
	inline namespace v2{//inline必须出现在命名空间第一次定义的地方，后续的inline可以省略，会进行隐式内联
		struct a{
			int x=1;
		};
		struct b{
			int y=100;
		};
	}
	namespace v1{
		struct c{
			int z=10;
		};
	}
}
void test1(){
	allver::a aa;//v2是内联的命名空间，所以可以直接进行使用
	cout<<aa.x<<endl;
	allver::v1::c cc;//v1不是内联的命名空间，所以只能使用完整的命名空间名字
	cout<<cc.z<<endl;
}
//未命名命名空间
namespace local{
	namespace{
		int i;
	}
}
void test2(){
	local::i=42;//通过外层命名空间的名字进行访问
	cout<<local::i<<endl;
}
//使用未命名的命名空间来代替文件的静态声明
template <const int &size>
class test{};
static int size1=10;
namespace{
	int size2=100;
}
void test3(){
	test<size1> t1;
	test<size2> t2;
}
//命名空间的别名
namespace _v1=allver::v1;
void test4(){
	_v1::c cc;
	cout<<cc.z<<endl;
}
//友元和实参相关的查找
namespace A{
	class C{
		//这两个函数隐式成为了命名空间A的成员
		friend void f2();//除非另有声明，不然无法找到
		friend void f(const C&);//根据实参的查找规则可以被找到
	};
}
void A::f(const C& c){
	cout<<"A::f(const C&)"<<endl;
}
void test5(){
	A::C obj;
	f(obj);//正确：通过在A::C中的友元声明找到A::f
	//f2();//错误，f2未被声明
}
//和实参相关的查找
namespace NS{
	class Quote{};
	void display(const Quote&){
		cout<<"void display(const Quote&)"<<endl;
	}
	class Bulk_item:public Quote{};
}
class Bulk_item:public NS::Quote{};
void test6(){
	Bulk_item book1;
	display(book1);//可以直接调用display函数，因为查询规则会查找book1所在的类及其基类所在的命名空间，并把和被调用函数同名的函数加入到候选集中
}
//using声明、using指示和重载
namespace libs{
	void print(int){}
	void print(double){
		cout<<"libs::print"<<endl;
	}
}
void print(double){
	cout<<"::print"<<endl;
}
void test7(){
	//这两条均会使print函数产生二义性
	//using namespace libs;
	//using libs::print;
	//print();
	::print(3.1);//使用全局作用域中的print(double)
	libs::print(3.1);//使用libs中的print(double)
}
int main(){
	//test1(); 	
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	test7();
}

