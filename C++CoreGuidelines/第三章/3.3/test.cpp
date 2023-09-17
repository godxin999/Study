/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/16 15:14:42
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <numeric>
#include <execution>
#include <vector>
#include <span>

//构建良好的接口应遵守以下规则
//1.接口明确
//2.接口精确并有强类型
//3.保持较低的参数数目
//4.避免相同类型却不相关的的参数相邻

void showRect(double a,double b,double c,double d){
	//参数名字具有误导性，且函数签名没有定义参数是什么以及参数的顺序信息
	//且参数为没有取值范围的双精度浮点数，需要在函数体中进行约束
}
/*
void showRect2(Point topLeft,Point bottomRight){
	//接受具体的点对象，对Point合法值的检测应在Point构造函数中
}
*/

void test1(){
	std::vector<std::string> strVec{"I","am","godxin999"};
	std::size_t res=std::transform_reduce(//获取所有元素的总长度
		std::execution::par,//采用并行执行策略
		strVec.begin(),strVec.end(),//range
		0,//初始值为0
		std::plus<>(),//然后调用规约函数(可调用对象)
		[](const std::string& str){//首先调用convert转换函数(可调用对象)
			return str.size();
		}
	);
	std::cout<<res<<std::endl;//12
}
//但是当你使用transform_reduce接受两个可调用实体的重载版本时，该版本需要六个模板参数和七个函数参数，其违背了第3、4条原则
//更好的办法是将transform_reduce分别定义，并通过管道运算符将他们组合起来：transform | reduce

//I.13 不要用单个指针来传递数组

template <typename T>
void copy_n(const T* p,T* q,int n){
	memcpy(q,p,n*sizeof(T));
}

int a[100];
int b[100];

void test2(){
	for(int i=0;i<100;++i){
		b[i]=i;
	}
	copy_n(a,b,101);//容易发生越界错误
}

template <typename T>
void copy(std::span<const T> src, std::span<T> dst) {
    std::copy(src.begin(), src.end(), dst.begin());
}

void test3(){
    int arr1[]={1,2,3};
    int arr2[]={3,4,5};
	copy<int>(arr1,arr2);
	for(const auto& i:arr2){
		std::cout<<i<<" ";
	}
}

int main(){

	//test1();
	//test2();
	test3();

	return 0;
}