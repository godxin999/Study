/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/4 10:37:52
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//派生类(内层作用域)中的名字隐藏基类(外层作用域)中的名字
struct Base{
	Base():mem(0){}
	void memfcn(){
		cout<<"Base::memfcn"<<endl;
	}
protected:
	int mem;
};
struct Derived:Base{
	Derived(int i):mem(i){}
	int get_mem(){
		return mem;//返回Derived::mem
	}
	int get_mem_base(){
		return Base::mem;
	}
	void memfcn(int){
		cout<<"Derived::memfcn"<<endl;
	}
protected:
	int mem;//隐藏了基类中的mem
};
void test1(){
	Derived d(42);
	cout<<d.get_mem()<<endl;//42
}
//使用域作用符来使用被隐藏的基类成员
void test2(){
	Derived d(42);
	cout<<d.get_mem_base()<<endl;//0
}
//派生类(内层作用域)中的名字隐藏基类(外层作用域)中的名字，同名函数成员被隐藏
void test3(){
	Derived d(42);
	Base b;
	b.memfcn();//Base::memfcn
	d.memfcn(10);//Derived::memfcn
	//d.memfcn();//错误，参数列表为空的memfcn的成员被隐藏了
	d.Base::memfcn();//Base::memfcn
}
class base{
public:
	virtual void fcn(){
		cout<<"base::fcn()"<<endl;
	}
};
class D1:public base{
public:
	void fcn(int){//隐藏了基类的fcn成员
		cout<<"D1::fcn(int)"<<endl;
	}
	virtual void f2(){//新的虚函数
		cout<<"D1::f2()"<<endl;
	}
};
class D2:public D1{
public:
	void fcn(int){//隐藏了D1::fcn(int)
		cout<<"D2::fcn(int)"<<endl;
	}
	void fcn(){//覆盖了base的虚函数fcn
		cout<<"D2::fcn()"<<endl;
	}
	void f2(){//覆盖了D1的虚函数f2
		cout<<"D2::f2()"<<endl;
	}
};

void test4(){
	base bobj;
	D1 d1obj;
	D2 d2obj;
	base *bp1=&bobj,*bp2=&d1obj,*bp3=&d2obj;
	bp1->fcn();//base::fcn()
	bp2->fcn();//base::fcn()
	bp3->fcn();//D2::fcn()
	D1 *d1p=&d1obj;
	D2 *d2p=&d2obj;
	//bp2->f2();//错误，以静态类型为准，base没有名为f2的成员
	d1p->f2();//D1::f2()
	d2p->f2();//D2::f2()
	base *p1=&d2obj;
	D1 *p2=&d2obj;
	D2 *p3=&d2obj;
	//p1->fcn(42);//错误，base中没有接受一个int的fcn成员
	p2->fcn(42);//D1::fcn(int)，静态绑定，因为fcn(int)不是虚函数
	p3->fcn(42);//D2::fcn(int)，静态绑定，因为fcn(int)不是虚函数
}
//通过using声明将所有重载实例添加到派生类作用域中
class A{
protected:
	void func(){
		cout<<"func()"<<endl;
	}
	void func(int a){
		cout<<"func(int a)"<<endl;
	}
};
class B:public A{
public:
	using A::func;//注意会改变访问属性
	void func(string& s){
		cout<<"func(string& s)"<<endl;
	}
};
void test5(){
	B b;
	b.func();//func()
	b.func(2);//func(int a)
	string s="100";
	b.func(s);//func(string& s)
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	test5();

}

