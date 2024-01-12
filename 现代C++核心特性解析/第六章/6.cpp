/*************************************************************************
	> File Name: 6.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/12 20:58:54
 ************************************************************************/

#include <iostream>

//字符串字面量是左值
void test1(){
	auto p=&"hello world";
}
//右值引用能够延长临时变量的生命周期
class X{
public:
	X(){
		std::cout<<"X()"<<std::endl;
	}
	X(const X&){
		std::cout<<"X(const X&)"<<std::endl;
	}
	~X(){
		std::cout<<"~X()"<<std::endl;
	}
	void show(){
		std::cout<<"show()"<<std::endl;
	}
};
X make_X(){
	X x1;
	return x1;
}
void test2(){
	//X x2=make_X();//make_X返回的是一个临时变量，这里调用了拷贝构造函数将临时变量拷贝给x2，然后临时变量被销毁
	//x2.show();
	X&& x3=make_X();//右值引用引用了make_X返回的临时变量，临时变量的生命周期被延长，所以不会被销毁，可以正常调用show
	x3.show();
}
//将亡值的产生途径
//1.使用static_cast将泛左值为右值引用
//2.临时量实质化，即纯右值转换为临时对象的过程
struct S{
	int a;
};
void test3(){
	S s1;
	S&& s2=static_cast<S&&>(s1);//使用static_cast将泛左值为右值引用
	int s3=S().a;//S()是一个纯右值，为了访问其成员变量a则需要一个泛左值，所以这里发生临时量实质化，将S()转换为将亡值
}
//万能引用
template<typename T>
void func(T&& t){
	std::cout<<std::boolalpha;
	std::cout<<std::is_rvalue_reference_v<decltype(t)><<std::endl;
}
void test4(){
	int a=10;
	func(a);//左值引用，false
	func(10);//右值引用，true
	func(std::move(a));//右值引用，true
	auto&& x1=a;
	auto&& x2=10;
	std::cout<<std::is_rvalue_reference_v<decltype(x1)><<std::endl;//false
	std::cout<<std::is_rvalue_reference_v<decltype(x2)><<std::endl;//true
}
//完美转发
template<typename T>
void func2(T&& t){
	func(std::forward<T>(t));
}
void test5(){
	int a=10;
	func2(a);//左值引用，false
	func2(10);//右值引用，true
	func2(std::move(a));//右值引用，true
}
//
struct K{
	K()=default;
	K(const K&)=default;
	K(K&&){
		std::cout<<"K(K&&)"<<std::endl;
	}
};
K f(K &&k){
	return k;//return或co_return中的返回对象如果是函数或者lambda表达式中的对象或者形参，那么编译器会使用移动代替复制
}
K f2(){
	K k;
	throw k;//throw抛出的对象如果是函数或者try代码块中的对象或，那么编译器会使用移动代替复制
}
void test6(){
	K k=f(K{});//K(K&&)
	try{
		f2();//K(K&&)
	}
	catch(...){

	}	
}

int main(){
	//test2();
	//test4();
	//test5();
	test6();



	return 0;
}