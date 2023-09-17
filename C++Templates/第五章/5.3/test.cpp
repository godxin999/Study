/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/17 10:52:11
 ************************************************************************/

#include <iostream>

//对于类模板，如果它的基类也是依赖于模板参数的，那么对它而言x是继承而来的，使用this->x和x也不一定是等效的
template <typename T>
class Base{
public:
	void bar(){
		std::cout<<"this is Base::bar()"<<std::endl;
	}
};
template <typename T>
class Derived:Base<T>{
public:
	void foo(){
		//bar();//不会被解析为Base中的bar，所以当使用定义于基类中的，依赖于模板参数的成员时，使用this->或者base<T>::来修饰它
		this->bar();
		Base<T>::bar();
	}
};


int main(){
	Derived<int> d;
	d.foo();

	return 0;
}