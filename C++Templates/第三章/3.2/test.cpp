/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/14 17:43:09
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
//函数模板的非类型参数
template <int Val,typename T>
T addValue(T x){
	return x+Val;
}
//通过前面的的模板参数推断出当前模板参数的类型
template <auto Val,typename T=decltype(Val)>
T addValue2(T x){
	return x+Val;
}
//通过如下方式确保传入的非类型模板参数的类型与函数模板的模板参数类型一致
template <typename T,T Val=T{}>
T foo(T x);

int main(){
	vector<int> vi(10,10);
	vector<int> vi2;
	transform(vi.begin(),vi.end(),back_inserter(vi2),addValue<10,int>);//需要指定模板参数T
	
	transform(vi.begin(),vi.end(),back_inserter(vi2),addValue2<20>);

	for(auto i:vi2){
		cout << i << endl;
	}

	return 0;
}