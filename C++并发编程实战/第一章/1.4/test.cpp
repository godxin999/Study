/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/24 16:51:26
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <thread>
using namespace std;

void hello(){//新线程始于此函数
	cout<<"Hello Concurrent World\n";
}

int main(){
	thread t(hello);//定义新线程
	t.join();//会等待和thread关联的线程
}

