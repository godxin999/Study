/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/24 15:15:20
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//完美转发临时变量
template <typename T>
void foo(T x){
	//使用auto&& 创建可以被转发的变量，这样可以避免对中间变量的多余拷贝
	auto&& val=get(x);
	set(std::forward<decltype(val)>(val));
}


int main(){





	return 0;
}
