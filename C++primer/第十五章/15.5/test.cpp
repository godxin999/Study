/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/4 9:11:49
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//派生类对基类对象的受保护成员没有访问权限
class Base{
	friend class Pal;
public:
	void pub_mem(){
		cout<<"pub_mem"<<endl;
	}
protected:
	int prot_mem;
private:
	char priv_mem;
};
class Sneaky:public Base{
	friend void clobber(Sneaky&);
	friend void clobber(Base&);
	int j;
};
void clobber(Sneaky &s){//正确，clobber能访问Sneaky对象的private和protected成员
	s.j=s.prot_mem=0;
}
/*
void clobber(Base &b){//错误，clobber不能访问Base的protected成员
	b.prot_mem=0;
}
*/
//公有，私有和受保护继承
struct Pub_Derv:public Base{
	int f(){
		return prot_mem;//正确，派生类能访问protected成员
	}
	/*
	char g(){
		return priv_mem;//错误，派生类不能访问private成员
	}
	*/
};
struct Priv_Derv:private Base{//private不影响派生类的访问权限
	int f1()const{
		return prot_mem;
	}
};
void test1(){
	Pub_Derv d1;
	Priv_Derv d2;
	d1.pub_mem();//正确，pub_mem在派生类Pub_Derv中是public的
	//d2.pub_mem();//错误，pub_mem在派生类Priv_Derv中是private的
}
struct Derived_from_Public:public Pub_Derv{
	int use_base(){
		return prot_mem;
	}
};
struct Derived_from_Private:public Priv_Derv{
	/*
	int use_base(){
		return prot_mem;//错误，Priv_Derv继承自Base的所有成员均为私有，所以Priv_Derv的派生类无法访问Base的prot_mem
	}
	*/
};
//友元关系不能继承，友元声明仅对声明友元的类有效
class Pal{
public:
	int f(Base b){
		return b.prot_mem;
	}
	/*
	int f2(Sneaky s){
		return s.j;//错误，Pal不是Sneaky的友元，所以不能访问Sneaky的private成员
	}
	*/
	int f3(Sneaky s){
		return s.prot_mem;//正确，Pal是Base的友元
	}
};
class D2:public Pal{
public:
	/*
	int mem(Base b){
		return b.prot_mem;//错误，友元不能继承
	}
	*/
};
//使用using声明改变对象可访问性
class base{
public:
	size_t size()const{
		return n;
	}
protected:
	size_t n;
};
class derived:private base{
public:
	using base::size;//声明为public，derived的对象可以使用size成员
protected:
	using base::n;//声明为protected，derived的派生类可以使用n
};


int main(){
	test1();
}

