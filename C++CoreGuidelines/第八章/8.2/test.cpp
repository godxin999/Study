/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/3 10:02:58
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <map>

//ES.5 保持作用域较小
//ES.6 在for语句的初始化式和条件中声明名字以限制其作用域
void f(){
	std::map<int,int> mii;
	/*if(auto result=mii.insert({1,1});result.second){
		//...
	}
	else{
		//...
	}*/
	if(auto [iter,succeeded]=mii.insert({1,1});succeeded){
		//...
	}
	else{
		//...
	}
}
//ES.7 常用的和局部的名字要短，不常用的和非局部的名字要长
//ES.8 避免看起来相似的名字
//ES.9 避免ALL_CAPS的名字(全大写加下划线常常用于宏)
//ES.10 每条声明进声明一个名字
//ES.11 使用auto来避免类型名字的多余重复(让编译器自动推导类型)
void f2(){
	auto a=5;
	auto b=5.0;
	auto c=a*5.0f;
	auto d=a*5;
	auto e=b*5;
}
//ES.12 不要在嵌套作用域中复用名字，这样会隐藏父作用域中的名字


//ES.20 始终初始化对象(使用auto，因为auto必须初始化)
//ES.21 不要在确实需要使用变量(常量)前就将其引入
//ES.22 在获得可用来初始化变量的值之前不要声明变量
//ES.23 优先使用{}初始化语法
void test1(){
	auto init1{1};//int
	auto init2={1};//std::initializer_list<int>
	//auto init3{1,2};//error，不是单个元素
	auto init4={1,2};//std::initializer_list<int>
}
struct MyInt{
	MyInt(int arg=0):i{arg}{}
	int i;
};
//{}可以避免解析问题
void test2(){
	MyInt m1(2023);
	//MyInt m2();//错误，被解析为函数声明
	MyInt m2{};
	std::cout<<m1.i<<std::endl;
	std::cout<<m2.i<<std::endl;
}
//{}初始化语法不允许窄化变换，()只会警告，而{}会报错
void test3(){
	//int i(3.14);//warning
	//int i2{3.14};//error
	//char c1(999);warning
	//char{999};//error
}

//ES.26 不要将同一个变量用于两个不想关的目的
//ES.28 使用lambda表达式进行复杂的初始化(尤其是const变量)
struct widget{};
const widget x=[&](){//通过引用捕获环境变量
	widget val;
	//do_somthing();
	return val;
}();//lambda表达式进行就地调用初始化const变量


int main(){
	//test1();
	//test2();
	test3();

	return 0;
}