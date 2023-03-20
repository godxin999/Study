/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/20 13:13:34
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

//内联命名空间
namespace allver{
	//如果每个命名空间都定义在同名的头文件中，则可以使用include来进行定义
	//#include "v1.h"
	//#include "v2.h"
	inline namespace v2{//inline必须出现在命名空间第一次定义的地方，后续的inline可以省略，会进行隐式内联
		struct a{
			int x=1;
		};
		struct b{
			int y=100;
		};
	}
	namespace v1{
		struct c{
			int z=10;
		};
	}
}
void test1(){
	allver::a aa;//v2是内联的命名空间，所以可以直接进行使用
	cout<<aa.x<<endl;
	allver::v1::c cc;//v1不是内联的命名空间，所以只能使用完整的命名空间名字
	cout<<cc.z<<endl;
}
//未命名命名空间
namespace local{
	namespace{
		int i;
	}
}
void test2(){
	local::i=42;//通过外层命名空间的名字进行访问
	cout<<local::i<<endl;
}
//使用未命名的命名空间来代替文件的静态声明
template <const int &size>
class test{};
static int size1=10;
namespace{
	int size2=100;
}
void test3(){
	test<size1> t1;
	test<size2> t2;
}



int main(){
	//test1(); 	
	//test2();
	test3();



}

