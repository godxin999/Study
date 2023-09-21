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

int main(){
	//test1();
	//test2();
	test3();


	return 0;
}