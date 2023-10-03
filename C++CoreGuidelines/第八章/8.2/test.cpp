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




int main(){




	return 0;
}