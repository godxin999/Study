/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/17 20:27:47
 ************************************************************************/

#include <iostream>
#include <cstring>


//用于处理裸数组或者字符串常量的模板
template <typename T,int N,int M>
bool less(T(&a)[N],T(&b)[M]){
	for(int i=0;i<N&&i<M;++i){
		if(a[i]<b[i])return true;
		if(b[i]<a[i])return false;
	}
	return N<M;
}

void test1(){
	int a[]={1,2,3};
	int b[]={1,2,3,4,5};
	std::cout<<less(a,b)<<std::endl;
	const char arr1[]="hello";
	const char arr2[]="world";
	std::cout<<less(arr1,arr2)<<std::endl;
}
//如果指向处理字符串，则可以定义如下形式
template <int N,int M>
bool less(const char (&a)[N],const char (&b)[M]){
	for(int i=0;i<N&&i<M;++i){
		if(a[i]<b[i])return true;
		if(b[i]<a[i])return false;
	}
	return N<M;
}

int main(){
	test1();


	return 0;
}