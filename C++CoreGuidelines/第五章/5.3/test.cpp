/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/20 23:26:22
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <map>
#include <string>

//C.20 如果能避免定义默认操作，就别定义它们
class Named_map{
public:
private:
	//因为std::string和std::map已经定义了默认构造和拷贝构造，所以编译器可以自己生成默认构造和拷贝构造
	std::map<int, int> mii;
	std::string name;
};
void test1(){
	Named_map nm;//默认构造
	Named_map nm2{nm};//拷贝构造
}
//C.21 如果定义或删除了任何默认操作(拷贝、移动和析构)，就对所有默认操作进行定义或删除
#include <cstddef>
class BigArray{
public:
	BigArray(std::size_t len):len_(len),data_(new int[len]){}
	~BigArray(){
		delete[] data_;
	}
private:
	std::size_t len_;
	int* data_;
};
void test2(){
	BigArray ba1(1000);
	BigArray ba2(1000);
	ba2=ba1;//默认的拷贝赋值操作对ba1进行了浅拷贝，仅复制了指针，没有复制指针指向的内容，导致两个对象指向同一块内存，析构时会出错
}
//C.22 让默认操作保持一致，因为编译器默认生成的拷贝操作都为浅拷贝，如果需要深拷贝，就需要自己定义拷贝操作
//C.41 构造函数应当创建完全初始化的对象，类不应该有init()函数，因为如果用户在init函数前调用了其他函数，就会报错
//C.42 如果构造函数无法构造出有效对象，则应该抛出异常
//C.43 确保可拷贝的(值类型)类有默认构造函数
//C.45 不要定义仅初始化数据成员的默认构造函数，而应该使用成员初始化器(C.48 使用常量来初始化时，优先选择类内初始化器，而不是构造函数的成员初始化)

class widget{
public:
	widget():width(640),height(480){}//bad
private:
	int width;
	int height;
};
class widget2{
public:
	widget2()=default;//good
private:
	//在类的主体中定义默认行为，明确定义的构造函数用来改变默认行为
	int width{640};
	int height{480};
};

//C.46 默认情况下，把单参数的构造函数声明为explicit，应避免定义转换构造函数
//C.47 按照成员声明的顺序定义和初始化成员变量

class Foo{
	int m1;
	int m2;
public:
	//因为m1在m2前面声明，所以m1先初始化，m2后初始化
	Foo(int x):m2{x},m1{++x}{
		std::cout<<"m1: "<<m1<<" m2: "<<m2<<std::endl;
	}
};
void test3(){
	Foo foo(10);//m1: 11 m2: 11
}

//C.49 在构造函数中优先使用初始化而不是赋值
class Good{
	std::string s;
public:
	Good(const std::string& s_):s(s_){}
};

//C.51 使用委托构造函数来表示类的所有构造函数的共同动作
#include <cmath>
class Degree{
public:
	Degree()=default;
	explicit Degree(int d){
		degree=d%360;
		if(degree<0)degree+=360;
	}
	explicit Degree(double d):Degree(static_cast<int>(std::ceil(d))){}//将构造函数委托给另一个构造函数
private:
	int degree=0;
};

//C.52 使用继承构造函数将构造函数导入不需要进一步显式初始化的派生类中
class Rec{

};
class Oper:public Rec{
	using Rec::Rec;//将Rec的构造函数导入Oper中
	//使用继承构造函数存在一个风险，如果你的派生类中有自己的成员，如果其没有类内初始化器，则它不会被初始化
};

//对于STL容器、std::string、和内置类型，拷贝/移动赋值对于自赋值是安全的。自动生成的拷贝/移动运算符对于自赋值也是安全的
class Bar{
	std::string	s;
	int i;
public:
	//多余的自赋值检查会让性能变差
	Bar& operator=(const Bar& rhs){
		s=rhs.s;
		i=rhs.i;
		return *this;
	}
	Bar& operator=(Bar&& rhs){
		s=std::move(rhs.s);
		i=rhs.i;
		return *this;
	}
};

//C.67 多态类应该抑制公开的拷贝和移动操作
//因为公开的拷贝和移动操作可能导致切片的发生

//C.33 如果类有具有所有权的指针成员，则需要定义析构函数
//如果类为指针的独占所有者，那么将指针放到std::unique_ptr<>中，否则应该把指针放到std::shared_ptr<>中
#include <memory>

struct myclass{
	std::unique_ptr<int> pint=std::make_unique<int>(2011);
};
//包含std::unique_ptr成员的类的拷贝操作均无效
void test4(){
	myclass mc;
	//myclass mc2(mc);
	//myclass mc3=mc2;
}

//C.35 基类的析构函数要么是public且virtual，要么是protected且非virtual
struct Base{
	virtual void f(){}
};
struct Derived:Base{
	std::string s{"a resource needing cleanup"};
	~Derived(){
		//cleanup
	}
};
void test5(){
	//编译器为Base生成了非虚的析构函数，通过Base的指针删除Derived实例是未定义行为，因为析构函数不是虚函数
	Base* b=new Derived;
	delete b;
}
//如果基类的析构函数为私有，那么就无法从这个类派生
//如果基类的析构函数为受保护的，那么你可以从这个类派生，但是只能使用子类
struct base{
protected:
	~base()=default;
};
struct derived:base{};
void test6(){
	//base b;//error，base的析构函数无法访问
	derived d;
}

//C.80 如果你明确使用默认语义，那么就使用=default
//C.81 如果想要禁用默认行为(且不需要替代方法)时使用=delete
//C.82 不要在构造函数和析构函数中调用虚函数(虚调用会被禁用)

//一个类型想要称为规范类型，那么其必须支持noexcept的swap函数，如果不提供，那么需要交换的标准库算法将会使用std::swap(使用移动语义)来交换对象
class X{
public:
	void swap(X& rhs)noexcept{
		std::swap(i,rhs.i);
	}
	void swap(X& lhs,X& rhs)noexcept{//提供非成员的swap函数
		lhs.swap(rhs);
	}
private:
	int i;
};

//拷贝并交换惯用法的swap函数基于拷贝语义而不是移动语义，其可能在拷贝时抛出std::bad_alloc，这与swap函数应该为noexcept的原则相违背

//一个类型想要称为规范类型，那么其必须支持noexcept的operator==并使操作数的类型对称
class MyInt{
	int num;
public:
	explicit MyInt(int i):num(i){}//explicit会破坏int到MyInt的隐式转换，所以提供两个重载的opreator==
	friend bool operator==(const MyInt& lhs,const MyInt& rhs)noexcept{
		return lhs.num==rhs.num;
	}
	friend bool operator==(int lhs,const MyInt& rhs)noexcept{
		return lhs==rhs.num;
	}
	friend bool operator==(const MyInt& lhs,int rhs)noexcept{
		return lhs.num==rhs;
	}
};

void test7(){
	std::cout<<std::boolalpha<<(MyInt(5)==5)<<" "<<(5==MyInt(5));//true true
}

//C.87 当心基类的operator==，因为其可能导致切片，而且如果基类和子类的operator==的签名不一样，即使基类的operator==是虚函数，也不会被覆盖
struct B{
	std::string name="";
	int num=0;
	bool operator==(const B& rhs)const noexcept{
		std::cout<<"operator==(const B& rhs)"<<std::endl;
		return name==rhs.name&&num==rhs.num;
	}
};
struct D:B{
	char c;
	bool operator==(const D& rhs)const noexcept{
		std::cout<<"operator==(const D& rhs)"<<std::endl;
		return B::operator==(rhs)&&c==rhs.c;
	}
};

void test8(){
	B b;
	D d;
	std::cout<<std::boolalpha<<(b==d)<<std::endl;//true
	//调用operator==(const B& rhs)，导致了切片
}

int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7();
	test8();

	return 0;
}