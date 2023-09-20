/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/20 16:20:59
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//为了检测传递进来的参数是否为数组，第一种方法为将模板定义为只能接受数组为参数
template <typename T,std::size_t L1,std::size_t L2>
void cmpArray1(T (&a)[L1],T (&b)[L2]){
	T* pa=a;//退化a
	T* pb=b;//退化b
	//...
}
//第二种方法为使用类型萃取来检测参数是不是一个数组
template <typename T>
requires std::is_array_v<T>
void cmpArray2(T&& a,T&& b){
	//...
}


int main(){
	int a[10];
	int b[10];
	cmpArray1(a,b);
	//cmpArray2(a,b);//指针类型并非数组类型
	return 0;
}