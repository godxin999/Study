/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/20 21:59:15
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//C.4 当且仅当函数需要直接访问类的内部表示时，才把函数变成成员
//=、[]、()和->运算符必须是成员函数

//C.5 将辅助函数和他支持的类放在同一个命名空间中

//C.7 不要在一条语句中定义类或枚举的同时声明该类型变量
struct Data{}data;//bad
struct Data2{};
Data2 data2;//good
//C.9 尽量减少成员的暴露
//你的类可能有两种接口，一种是用于外部的public接口，另一种为用于派生类的protected接口

int main(){
	return 0;
}