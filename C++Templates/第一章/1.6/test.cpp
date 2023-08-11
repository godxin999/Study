/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/11 19:19:06
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <array>

template <typename T1, typename T2>
constexpr auto max(T1 a, T2 b){
	return b < a ? a : b;
}


int main(){
	int a[::max(sizeof(char),1000u)];
	std::array<int, ::max(sizeof(char),1000u)> arr;//传递的1000是无符号数，可以避免直接比较一个有符号数和无符号数时的warning


}