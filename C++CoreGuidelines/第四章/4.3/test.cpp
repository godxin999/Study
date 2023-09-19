/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/19 13:34:12
 ************************************************************************/

#include <iostream>
#include <utility>
#include <memory>

//在应用层使用std::move的目的为所有权的转移
void test1(){
	auto p=std::make_unique<int>(42);
	auto q=std::move(p);//q接管了p中资源的所有权
}
class Foo{
public:
	explicit Foo(int v):val(v){

	}
	~Foo(){
		std::cout<<"Foo destructor"<<std::endl;
	}
private:
	int val;
};
void func(std::unique_ptr<Foo> pFoo){

}
void test2(){
	auto p=std::make_unique<Foo>(42);
	std::cout<<"Before func(std::move(p))"<<std::endl;
	func(std::move(p));
	std::cout<<"After func(std::move(p))"<<std::endl;
// Before func(std::move(p))
// Foo destructor 函数参数接管了p中资源的所有权，所以其会在函数结束时进行析构
// After func(std::move(p))
}

int main(){
	//test1();
	test2();



	return 0;
}