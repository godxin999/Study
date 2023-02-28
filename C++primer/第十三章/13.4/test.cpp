/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/27 12:45:38
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include "Folder.h"
using namespace std;

int main(){
	Message m("hello");
	Folder f;
	m.save(f);
	Folder f2(f);
	Message m2("world");
	Message m3 = m;
	swap(m, m2);
	m = m;
	f = f;
}

