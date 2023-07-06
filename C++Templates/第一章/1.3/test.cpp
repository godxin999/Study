/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/7/6 22:32:33
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <type_traits>
#include <cstring>
using namespace std;
//返回类型推断
template <typename T1,typename T2>
auto max(T1 a,T2 b){
	return b > a ? b : a;
}
//C++11使用尾置返回类型，返回类型由三目运算符?:决定
template <typename T1,typename T2>
auto max(T1 a,T2 b)->decltype(b > a ? b : a){
	return b > a ? b : a;
}
template <typename T1,typename T2>
auto max(T1 a,T2 b)->decltype(true ? b : a){//可以使用true作为条件
	return b > a ? b : a;
}
//因为T可能为引用类型，所以需要进行类型萃取
template <typename T1,typename T2>
auto max(T1 a,T2 b)->std::decay_t<decltype(true ? b : a)>{
	return b > a ? b : a;
}
//将返回类型设置为公共类型
template <typename T1,typename T2>
std::common_type_t<T1,T2> max(T1 a,T2 b){//common_type也是类型萃取，其产生两个模板参数的公共类型
	return b > a ? b : a;
}


int main(){







}

