/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/9 10:01:46
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//优先选择基于范围的for，且如果有一个明显的循环变量，首选for语句而不是while语句
//优先考虑将循环变量声明在for语句的初始化部分
//ES.75 避免使用do和goto语句
//ES.77 尽量不要在循环中使用break和continue
//优先使用算法而不是原始循环
#include <vector>
#include <chrono>
#include <execution>

void test1(){
	std::vector<int> v1{-10,5,0,3,-20,31};
	auto v2=v1;
	using namespace std::chrono;
	auto start = high_resolution_clock::now();
	std::sort(std::execution::par, v1.begin(), v1.end());//并行
	auto end = high_resolution_clock::now();
	std::cout << duration_cast<nanoseconds>(end - start).count() << std::endl;
	start = high_resolution_clock::now();
	std::sort(std::execution::par_unseq,v2.begin(), v2.end());//并行加向量化
	end = high_resolution_clock::now();
	std::cout << duration_cast<nanoseconds>(end - start).count() << std::endl;
}

//ES.78 不要依赖switch的隐式直落行为
void test2(){
	int a=2;
	switch(a){
		case 1:{
			std::cout<<1<<std::endl;
			break;
		}
		case 2:{
			std::cout<<2<<std::endl;
			[[fallthrough]];//[[fallthrough]]属性，表示这里是故意直落的
		}
		case 3:{
			std::cout<<3<<std::endl;
			break;
		}
	}
}
//ES.79 仅使用default来处理一般情况




int main(){
	//test1();
	test2();



	return 0;
}