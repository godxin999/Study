/*************************************************************************
	> File Name: item13.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/18 16:20:03
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
//在C++11中使用非成员cbegin
template <class C>
auto cb(const C& container)->decltype(std::begin(container)){
	return std::begin(container);//对const容器调用非成员函数begin()将产出const_iterator
}
//使用const_iterator
template <typename C,typename V>
void findAndInsert(C& container,const V& targetVal,const V& insertVal){
	//using std::cbegin;//C++14加入
	using std::cend;
	auto it=std::find(cb(container),cend(container),targetVal);
	container.insert(it,insertVal);
}
void test1(){
	vector<int> v={1,2,4,5};
	findAndInsert(v,4,3);
	for(const auto &i:v){
		cout<<i<<endl;
	}
}
int main(){
	test1();
}