/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/9 10:23:43
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//ES.100 不要混合有符号和无符号的算数运算
//ES.101 使用无符号类型进行位操作
//ES.102 使用有符号类型进行算数运算
//ES.106 不要试图通过使用unsigned来避免负值
void test1(){
	short x{0};
	int max{100000};
	std::size_t count{0};
	while(x<max&&count<20){
		std::cout<<x<<" ";
		x=x+10000;//溢出
		//x={x+10000};//花括号表达式检查窄化转换，从而检测到溢出
		++count;
	}
	std::cout<<std::endl;

}
//ES.103/ES.104 避免上/下溢
//ES.105 不要除以0
int main(){
	//test1();




	return 0;
}