/*************************************************************************
	> File Name: item31.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/17 10:03:51
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//避免使用lambda表达式的默认捕获
int var_global=9;
class bar{
public:
	bar(int val):var_datamember(val){}
auto func(int var_localparam){
	static int var_localstatic=10;
	int var_local=12;
	/*
	return [&](){//使用引用捕获，对局部变量进行引用，造成悬空引用
		printf("var_localparam:%d,var_local:%d\n",var_localparam,var_local);
		printf("var_localstatic:%d,var_datamember:%d,var_global:%d\n",var_localstatic,var_datamember,var_global);
	};
	*/
	return [=](){//只有在对象内存未被释放的情况下才正确
		printf("var_localparam:%d,var_local:%d\n",var_localparam,var_local);
		printf("var_localstatic:%d,var_datamember:%d,var_global:%d\n",var_localstatic,var_datamember,var_global);
	};

}
private:
	int var_datamember;
};
void test1(){
	bar b(0);
	auto f=b.func(6);
	f();
}
auto getf(){
	bar b(0);
	auto f=b.func(6);
	return f;
}
void test2(){
	auto f=getf();
	f();
}
class Bar{
public:
	Bar(int divisor):divisor_(divisor){}
	auto bindDivisor(){
		//divisor_只是个声明，实际捕获的是this指针，在对象内存释放后会产生未定义行为
		/*
		return [=](int value)->bool{
			return value%divisor_==0;
		};
		*/
		//使用局部变量拷贝然后捕获这个局部变量
		/*
		auto divisorCopy=this->divisor_;
		return [divisorCopy](int value)->bool{
			return value%divisorCopy==0;
		};
		*/
		//C++14初始化捕获
		return [divisor_=divisor_](int value)->bool{
			return value%divisor_==0;
		};
	}
private:
	int divisor_;
};
auto getF(){
	Bar b(10);
	auto f=b.bindDivisor();
	return f;
}
void test3(){
	auto f=getF();
	cout<<boolalpha<<f(10)<<endl;//true
}
//lambda捕获静态局部变量
auto bindDivisor(){
	static int divisor=10;
	divisor++;
	printf("divisor:%d(%p)\n",divisor,&divisor);
	//不同的返回值的divisor应该不同，但是使用默认捕获会导致它们的divisor指向同一块内存
	/*
	return [=](int value)->bool{
		printf("lambda divisor:%d(%p)\n",divisor,&divisor);
		return value%divisor==0;
	};
	*/
	//C++14初始化捕获
	return [divisor=divisor](int value)->bool{
		printf("lambda divisor:%d(%p)\n",divisor,&divisor);
		return value%divisor==0;
	};
}
void test4(){
	auto f=bindDivisor();
	f(10);
	auto f2=bindDivisor();
	f2(10);
	f(10);
}
int main(){
	//test1();
	//test2();
	//test3();
	test4();


}