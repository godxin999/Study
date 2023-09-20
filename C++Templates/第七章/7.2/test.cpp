/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/20 11:00:13
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <type_traits>
#include <utility>

class myClass{
public:
	std::string s="myclass";
	myClass(const std::string& s):s(s){}
	myClass()=default;
	myClass(const myClass& rhs){
		s=rhs.s;
		std::cout<<"copy constructor"<<std::endl;
	}
	myClass(myClass&& rhs){
		s=std::move(rhs.s);
		std::cout<<"move constructor"<<std::endl;
	}
	myClass& operator=(const myClass& rhs){
		s=rhs.s;
		std::cout<<"copy assignment"<<std::endl;
		return *this;
	}
	myClass& operator=(myClass&& rhs){
		s=std::move(rhs.s);
		std::cout<<"move assignment"<<std::endl;
		return *this;
	}
	friend std::ostream& operator<<(std::ostream& os,const myClass &m){
		os<<m.s;
		return os;
	}
};

myClass returnMyClass(){
	return myClass();
}

//按const&传递不会发生拷贝
template <typename T>
void print(const T& arg){
	std::cout<<arg<<std::endl;
}
void test1(){
	myClass m;
	print(m);
	print(myClass("hello"));
	print(returnMyClass());
	print(std::move(m));
}
//按引用传递不会发生类型退化
void test2(){
	const char a[]="hello";
	print(a);
}

template <typename T>
void print2(T& arg){
	std::cout<<arg<<std::endl;
}
//非const引用不允许绑定到右值
void test3(){
	myClass m;
	print2(m);
	//print2(myClass("hello"));
	//print2(returnMyClass());
	//print2(std::move(m));
}
//按引用传递的数组类型不会退化，且可以处理数组的长度
template <typename T>
void out(T& arg){
	if(std::is_array_v<T>){
		std::cout<<std::extent_v<T><<std::endl;
	}
}
void test4(){
	const char a[]="hello";
	out(a);//6
}
//如果传递的参数是const的，那么arg的类型就会被推导为const引用，所以这时可以传递一个右值作为参数
void test5(){
	out("hi");//T被推导为 const char[3]
}
//禁用非const引用传递const对象
template <typename T>
requires (!std::is_const_v<T>)
void out2(T& arg){
	std::cout<<arg<<std::endl;	
}
void test6(){
	const char a[]="hello";
	//out2(a);
	//out2("hi");
}
//使用万能引用
template <typename T>
void print3(T&& arg){
	std::cout<<arg<<std::endl;
}
void test7(){
	myClass m;
	print3(m);
	print3(myClass("hello"));
	print3(returnMyClass());
	print3(std::move(m));
}
//因为万能引用会把T隐式推断为引用，所以不能使用T在模板中声明未初始化的局部变量
template <typename T>
void foo(T&& arg){
	T x;
}
void test8(){
	foo(5);
	int i=100;
	//foo(i);//T被推导为int&，引用必须在声明时初始化
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