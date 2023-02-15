/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/15 22:35:01
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <deque>
#include <stack>
#include <vector>
using namespace std;
//定义适配器
void test1(){
	deque<int> deq;
	vector<int> vec;
	stack<int> stk(deq);//从deque拷贝元素到stack中，这里stack是基于deque实现的
	stack<int,vector<int> > istk;//基于vector实现的stack
	stack<int,vector<int> > istk2(vec);//把vector中的元素拷贝到基于vector实现的stack中
}
int main(){
	test1();
}

