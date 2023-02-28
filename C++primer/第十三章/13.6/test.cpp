/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/28 13:28:40
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
//右值引用
void test1(){
	int i=42;
	int &r=i;//正确，r引用i
	//int &&rr=i;//错误，不能把一个右值引用绑定到一个左值上
	//int &r2=i*42;//错误，i*42是一个右值
	const int &r3=i*42;//正确，可以将一个const的引用绑定到一个右值上
	int &&rr2=i*42;//正确，将rr2绑定到乘法结果
}
//变量是左值
void test2(){
	//不能将一个右值引用绑定到右值引用类型的变量上
	int &&rr1=42;//正确，字面值常量是右值
	//int &&rr2=rr1;//错误，表达式rr1是左值
	int &&rr3=move(rr1);//move函数告诉编译器我们把一个左值当作右值来处理，显式地把一个左值转换为对应的右值引用类型，
	rr1=100;//对移动后的对象，除了赋值或销毁它之外将不再被使用
	cout<<rr1<<" "<<rr3<<endl;
}
//合成的移动操作
struct X{
	int i;//内置类型可以移动
	string s;//s定义了自己的移动操作
};
struct hasX{
	X mem;//X有合成的移动操作
};
void test3(){
	X x,x2=move(x);//使用合成的移动构造函数
	hasX hx,hx2=move(hx);//使用合成的移动构造函数
}
//没有移动构造函数，右值也被拷贝
class Foo{
public:
	Foo()=default;
	Foo(const Foo&)=default;//拷贝构造函数，因为定义了拷贝构造函数，此时编译器将不会合成移动构造函数
private:
	int x;
	string s;
};
void test4(){
	Foo x;
	Foo y(x);//拷贝构造函数，x是一个左值
	Foo z(move(x));//拷贝构造函数，因为没有定义移动构造函数，move(x)返回一个绑定到x的Foo&&，因为我们可以将Foo&&转换为const Foo&，所以z的初始化将通过拷贝构造函数进行
}
//引用限定符
class F{
public:
	F& operator=(const F&)&;//只能向可修改的左值赋值
	F func()const&;//可以同时使用const限定符和引用限定符，其中const限定符在前
};
F& F::operator=(const F& rhs)&{//引用限定符需要同时出现在声明和定义中，对于&限定的函数，我们只能将其用于左值，对于&&限定的函数，只能用于右值
	return *this;
}
//重载和引用函数
class A{
public:
	A sorted()&&;//定义两个以上同名同参数列表的函数时，要么都加上引用限定符，要么都不加
	A sorted()const &;
private:
	vector<int> data;
};
A A::sorted()&&{//本对象为右值，可以直接排序
	sort(data.begin(),data.end());
	return *this;
}
A A::sorted()const&{//本对象是一个const或是一个左值，哪种情况都不能对原址排序
	A ret(*this);//拷贝一个副本
	sort(ret.data.begin(),ret.data.end());//对副本进行排序
	return ret;//返回副本
}


int main(){
	//test1();
	//test2();
	//test3();
	test4();


}

