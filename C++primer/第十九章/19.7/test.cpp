/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/25 16:02:57
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//局部类
int a,val;
void test1(int val){
	static int si;
	enum Loc{
		a=1024,b
	};
	struct Bar{//局部类
		Loc locVal;//正确，使用一个局部类型名
		int barVal;
		void fooBar(Loc l=a){//局部类的所有成员都要完整的定义在类的内部
			//barVal=val;//错误，val是test1的局部变量，不能进行访问
			barVal=::val;//正确，使用全局对象
			barVal=si;//正确，使用静态局部对象
			barVal=b;//正确，使用一个枚举成员
		}
	};
}
//嵌套的局部类
void test2(){
	class Bar{//局部类
	public:
		class Nested;//嵌套类声明
	};
	class Bar::Nested{//嵌套在局部类中的类的定义可以在局部类之外，但必须定义在与局部类相同的作用域中，所以该类也是一个局部类，要满足所有成员均完整的定义在类的内部
	};
}
//局部类使用
void test3(){
	class f{
	public:
		void operator()(int n){
			cout<<n<<endl;
		}
		void test(){
			cout<<"test"<<endl;
		}
	}print;
	print(100);//100
	print(1000);//1000
	print.test();//test
}
int main(){
	//test1(val);
	//test2();
	test3();
}