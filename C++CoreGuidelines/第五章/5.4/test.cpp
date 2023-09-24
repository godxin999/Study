/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/24 19:52:25
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//C.120 仅使用类的层次结构来表达具有内在层次的概念
//如果某样东西本质上是不分层的，那就不应该使用分层的方式对齐进行建模
class DrawableUIElement{
public:
	virtual void render() const = 0;
};
class AbstractButton:public DrawableUIElement{
public:
	virtual void onClick() = 0;
};
class PushButton:public AbstractButton{
	void render()const override{

	}
	void onClick() override{

	}
};
class CheckBox:public AbstractButton{
	void render()const override{

	}
	void onClick() override{

	}
};
//C.121 如果基类被当作接口使用，那就把它变成抽象类(由公共的纯虚函数组成，没有数据成员，且有默认/空的析构函数)
class V{
public:
	virtual void f()=0;
	~V()=default;
};
class VB:public V{
public:
	void f() override{
		std::cout<<"VB::f()"<<std::endl;
	}
};
//C.122 当需要完全分离接口和实现时，以抽象类作为接口
//C.126 抽象类通常不需要构造函数
//C.128 虚函数应该指定为override、final和virtual三者之一
struct B{
	//只在声明时指定为virtual
	virtual void test(){

	}
	virtual void test2(){

	}
};
struct D:public B{
	//重写基类的虚函数时声明为override
	void test()override{

	}
	//子类不再重写基类的虚函数时声明为final
	void test2()final{

	}
};
//C.130 若要对多态类进行深拷贝，应使用虚函数clone，而不是公开的拷贝构造/赋值
#include <memory>

struct Base{
	Base()=default;
	virtual ~Base()=default;
	virtual std::unique_ptr<Base> clone(){
		return std::unique_ptr<Base>(new Base(*this));
	}
	virtual std::string getName(){
		return "Base";
	}
protected:
	Base(const Base&)=default;
	Base& operator=(const Base&)=default;
};
struct Derived:public Base{
	Derived()=default;
	~Derived()=default;
	std::unique_ptr<Base> clone()override{
		return std::unique_ptr<Base>(new Derived(*this));
	}
	std::string getName()override{
		return "Derived";
	}
};
void test1(){
	auto b1=std::make_unique<Base>();
	auto b2=b1->clone();
	std::cout<<b1->getName()<<std::endl;
	std::cout<<b2->getName()<<std::endl;
	auto d1=std::make_unique<Derived>();
	auto d2=d1->clone();
	std::cout<<d1->getName()<<std::endl;
	std::cout<<d2->getName()<<std::endl;
}

//C.131 避免无意义的取值和设值函数
class point{
public:
	point(int xx,int yy):x(xx),y(yy){}
	//如果取值和设值函数没有对数据成员提供额外的语义，那它们就没有价值
	int getX()const{
		return x;
	}
	int getY()const{
		return y;
	}
	void setX(int xx){
		x=xx;
	}
	void setY(int yy){
		y=yy;
	}
private:
	int x;
	int y;
};
//x和y仅仅是数值，直接使用struct
struct point2{
	int x{0};
	int y{0};
};
//C.133 避免protected数据
//protected数据会破坏封装性，即protected数据是类层次结构范围中的一种全局数据，而非const的全局数据不好
//C.134 确保所有非const数据成员都具有相同的访问级别
//不表示对象不变式的非const数据成员应为public，表示对象不变式的非const数据成员应为private(不变式即一组成员，它们不能自己独立变化，而只能作为一个整体来变化，如时间(y-m-d))

//C.129 在设计类的层次结构时，要区分实现继承和接口继承
//接口继承关注的是接口和实现的分离，这样派生类的修改就可以不影响基类的用户，实现继承则使用继承来扩展现有的功能，从而支持新功能
//纯接口继承指的是基类只有纯虚函数
//使用双重继承/PImpl惯用法来兼顾两者(用接口分层的稳定接口和实现继承的代码重回)
//以下为双重继承的解决方案
struct Color{
	int r{0};
	int g{0};
	int b{0};
	int a{0};
};
struct Point{
	int x;
	int y;
};
class Shape{
public:
	virtual Point center()const=0;
	virtual Color color()const=0;
	virtual void rotate(int)=0;
	virtual void move(Point p)=0;
	virtual void redraw()const=0;
	//...
};
class Circle:public virtual Shape{
	virtual int radius()=0;
	//...
};
namespace Impl{
	class Shape;
	class Circle;
}
class Impl::Shape:public virtual ::Shape{
public:
	//构造函数，析构函数
	Point center()const override{
		return Point{};
	}
	Color color()const override{
		return Color{};
	}
	void rotate(int)override{}
	void move(Point p)override{}
	void redraw()const override{}
	//...
};
class Impl::Circle:public virtual ::Circle,public virtual Impl::Shape{
public:
	//构造函数，析构函数
	int radius()override{
		return 0;
	}
	//...
};
void test2(){
	//层次结构为:Circle->Shape
	//Impl::Shape->Shape
	//Impl::Circle->Impl::Shape和Circle
    Impl::Circle circle;
    circle.redraw();
    std::cout << "Radius: " << circle.radius() << std::endl;
}
//C.135 使用多重继承来表示多个不同的接口
//class iostream:public istream,public ostream{};
//C.138 使用using为派生类及其基类创建重载集
class base{
public:
	void f(int){}
	void f(double){}
};
class derived:public base{
public:
	using base::f;//使用using导入基类的所有重载函数，防止了派生类的重载函数隐藏了基类的重载函数
	void f(std::string){}
};
//C.140 不要为虚函数和他的覆盖函数提供不同的默认参数
//C.146 在穿越层次不可避免时，应使用std::dynamic_cast
//C.147/C.148 当对引用类型使用std::dynamic_cast时，失败时会抛出std::bad_cast异常，对指针类型使用std::dynamic_cast时，失败时会返回nullptr，如果失败是一种选择，则对指针类型使用std::dynamic_cast，否则对引用类型使用std::dynamic_cast
//C.152 永远不要把指向派生类对象数组的指针赋值给指向基类的指针
struct base2{
	int x;
};
struct derived2:public base2{
	int y;
};
void test3(){
	derived2 a[]={{1,2},{3,4}};
	base2* p=a;
	p[1].x=7;//a退化为&a[0]，然后被转换为b*，根据指针运算规则，这里更改的是a[0]的y
	std::cout<<a[0].x<<" "<<a[0].y<<std::endl;//1 7
}

int main(){
	//test1();
	//test2();
	test3();



	return 0;
}