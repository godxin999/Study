/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/21 16:01:05
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <array>

constexpr bool isPrime(unsigned p){
	for(unsigned d=2;d*2<p;++d){
		if(p%d==0){
			return false;
		}
	}
	return p>1;
}
//可以通过模板偏特化进行路径选择
template <unsigned SZ,bool=isPrime(SZ)>
struct Helper;

template <unsigned SZ>
struct Helper<SZ,true>{

};

template <unsigned SZ>
struct Helper<SZ,false>{

};

template <typename T,std::size_t SZ>
long foo(const std::array<T,SZ>& coll){
	Helper<SZ> h;
}

int main(){

	return 0;
}