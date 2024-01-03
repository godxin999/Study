/*************************************************************************
	> File Name: 2.1.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/3 22:39:02
 ************************************************************************/

#include <iostream>

//内联命名空间，inline可以出现在除第一个命名空间外的任何命名空间前面
namespace parent{
	namespace V1{
		void foo(){
			std::cout<<"parent::V1::foo()"<<std::endl;
		}
	}
	//内联命名空间将空间内函数和类型导出到父命名空间
	//可以帮助用户无缝升级代码，只需要定义新版本的命名空间，并将其内联，然后把原来的命名空间的inline去掉即可
	inline namespace V2{
		void foo(){
			std::cout<<"parent::V2::foo()"<<std::endl;
		}
	}
}
void test1(){
	parent::V1::foo();//V1版本需要显式调用
	parent::foo();
}
//C++17支持使用::描述命名空间
namespace A::B::C{
	void foo(){
		std::cout<<"A::B::C::foo()"<<std::endl;
	}
}
//等价于如下形式
/*
namespace A{
	namespace B{
		namespace C{
			void foo(){
				std::cout<<"A::B::C::foo()"<<std::endl;
			}
		}
	}
}
*/
void test2(){
	A::B::C::foo();
}
//C++20支持::来定义嵌套命名空间
namespace A1::B::inline C{
	void foo(){
		std::cout<<"A1::B::C::foo()"<<std::endl;
	}
}
namespace A2::inline B::C{
	void foo(){
		std::cout<<"A2::B::C::foo()"<<std::endl;
	}

}
void test3(){
	A1::B::foo();
	A2::C::foo();
}

int main(){
	//test1();
	//test2();
	test3();

	return 0;
}