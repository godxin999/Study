/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/30 11:51:16
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//R.10 避免malloc()和free()
//因为malloc函数仅分配内存，不会调用构造函数，而new会调用构造函数，同理free仅释放内存，不会调用析构函数，而delete会调用析构函数
//R.11 避免显式效用new和delete
//R.12 立即将显式资源分配的结果交给一个管理者对象
#include <memory>
struct MyInt{
	explicit MyInt(int i):val(i){}
	~MyInt(){
		std::cout<<"myInt destructor and val is "<<val<<"."<<std::endl;
	}
	int val;
};
void test1(){
	MyInt* myInt=new MyInt(2023);
	std::unique_ptr<MyInt> p1=std::unique_ptr<MyInt>(myInt);
	std::unique_ptr<MyInt> p2=std::unique_ptr<MyInt>(myInt);
	//myInt destructor and val is 2023.
	//myInt destructor and val is -1561747584.
	std::unique_ptr<MyInt> p3=std::make_unique<MyInt>(2023);//good
}
//R.13 在一条表达式语句中最多进行一次显式资源分配
void func(std::shared_ptr<MyInt> sp1,std::shared_ptr<MyInt> sp2){
	//...
}
void test2(){
	func(std::shared_ptr<MyInt>(new MyInt(2023)),std::shared_ptr<MyInt>(new MyInt(2024)));
	//在C++17前，编译器可以自己决定先为两个对象分配内存然后再构造这两个对象，此时如果一个构造函数抛出异常，那么就可能导致内存泄漏
	//在C++17后，求值顺序得到保证，func中的子表达式会在一项求值完成后再进行下一项，不过先后顺序没有规定
}




int main(){
	//test1();
	test2();



	return 0;
}