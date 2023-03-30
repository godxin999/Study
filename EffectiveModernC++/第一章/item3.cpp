/*************************************************************************
	> File Name: item3.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/30 22:29:49
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <type_traits>
#include <vector>
using namespace std;
//使用decltype计算返回类型
template <typename Container,typename Index>//C++11
auto authAndAccess(Container &c,Index i)->decltype(c[i]){//使用尾置返回类型
	return c[i];
}
template <typename Container,typename Index>//C++14
auto authAndAccess2(Container &c,Index i){//忽略尾置返回类型，编译器会从函数实现中推导出函数的返回类型
	return c[i];//operator[]返回T&类型，模板对其进行推导会忽略引用，最后返回一个T类型
}
void test1(){
	vector<int> v={1,2,3,4,5,6};
	cout<<authAndAccess2(v,5)<<endl;
}
void test2(){
	vector<int> v={1,2,3,4,5,6};
	cout<<boolalpha<<is_same<decltype(authAndAccess2(v,5)),int>::value<<endl;
}
//返回c[i]真正的类型，但是不能传递右值容器
template <typename Container,typename Index>
decltype(auto) authAndAccess3(Container &c,Index i){
	return c[i];
}
void test3(){
	vector<int> v={1,2,3,4,5,6};
	cout<<boolalpha<<is_same<decltype(authAndAccess3(v,5)),int&>::value<<endl;
}
//decltype(auto)
void test4(){
	int i=10;
	const int &ci=i;
	auto mi=ci;//首先忽略引用，然后忽略顶层const
	decltype(auto) mi2=ci;//获取完整类型
	cout<<boolalpha<<is_same<decltype(mi),int>::value<<endl;
	cout<<is_same<decltype(mi2),const int&>::value<<endl;
}
//使用通用引用支持传入右值
template <typename Container,typename Index>
decltype(auto) authAndAccess4(Container &&c,Index i){
	return std::forward<Container>(c)[i];//使用std::forward实现通用引用
}
vector<int> getvec(){
	vector<int> v={1,2,3,4,5,6};
	return v;
}
void test5(){
	cout<<boolalpha<<is_same<decltype(authAndAccess4(std::move(getvec()),5)),int>::value<<endl;
	cout<<boolalpha<<is_same<decltype(authAndAccess4(static_cast<vector<int>&&> (getvec()),5)),int>::value<<endl;
	authAndAccess4(getvec(),5)=100;//返回值为一个左值引用，依旧为一个空悬引用
	auto v=authAndAccess4(getvec(),5);//获取第五个元素的拷贝
}


int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	test5();
}
