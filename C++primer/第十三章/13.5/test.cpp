/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/28 8:07:04
 ************************************************************************/

#include "StrVec.h"
using namespace std;

int main(){

	StrVec v;
	string s("some string or another");
	v.push_back(s);//copy，调用push_back(const string&)
	v.push_back("done");//move，调用push_back(string&&)




}

