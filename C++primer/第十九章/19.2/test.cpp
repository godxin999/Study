/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/22 19:25:57
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <typeinfo>
using namespace std;
//指针类型的dynamic_cast
class Base{
public:
	virtual void print(){
		cout<<"Base"<<endl;
	}
};
class Derived:public Base{
public:
	virtual void print(){
		cout<<"Derived"<<endl;
	}
};
void test1(){
	Base* bp=new Derived;//指向Derived对象的Base*指针
	if(Derived *dp=dynamic_cast<Derived*>(bp)){//转换为Derived*指针
		cout<<"success"<<endl;
	}
	else{
		cout<<"fail"<<endl;
	}
}
//引用类型的dynamic_cast
void test2(){
	Derived d;
	Base &b = d;
	try{
		const Derived &d=dynamic_cast<const Derived&> (b);
	}
	catch(bad_cast){
		cout<<"bad_cast"<<endl;
	}
}
//typeid运算符
void test3(){
	Derived *dp=new Derived;
	Base *bp=dp;
	if(typeid(*bp)==typeid(*dp)){
		cout<<"have same type"<<endl;
	}
	if(typeid(*bp)==typeid(Derived)){
		cout<<"have same type"<<endl;
	}
	cout<<typeid(bp).name()<<endl;//class Base * __ptr64，传一个指针给typeid会返回静态编译时类型
}
//使用RTTI
class base{
	friend bool operator==(const base&,const base&);
public:
	int i=1;
protected:
	virtual bool equal(const base &rhs)const{
		return i==rhs.i;
	}
};
class derived:public base{
public:
	int j=10;
protected:
	bool equal(const base &rhs)const{
		auto r=dynamic_cast<const derived&> (rhs);
		return i==r.i&&j==r.j;
	}
};
bool operator==(const base &lhs,const base &rhs){
	return typeid(lhs)==typeid(rhs)&&lhs.equal(rhs);
}
void test4(){
	base b1;
	derived d1,d2;
	base &b2=d2;
	cout<<boolalpha<<(b2==d2)<<endl;//true
	cout<<(d1==d2)<<endl;//true
	cout<<(b1==d1)<<endl;//false
}
//typeid.name
void test5(){
	int arr[10];
	Derived d;
	Base *p=&d;
	cout<<typeid(42).name()<<endl;//int
	cout<<typeid(arr).name()<<endl;//int [10]
	cout<<typeid(std::string).name()<<endl;//class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >
	cout<<typeid(p).name()<<endl;//class Base * __ptr64
	cout<<typeid(*p).name()<<endl;//class Derived
}

int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	test5();
}