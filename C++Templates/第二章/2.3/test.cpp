/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/11 20:41:31
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include "..\2.1\stack.h"

int main(){
	Stack<std::pair<int,int>>spii;
	spii.push({4,5});
	spii.push({6,7});
	std::cout<<spii.top().first<<std::endl;
	std::cout<<spii.top().second<<std::endl;
	//spii.printOn(std::cout);//error
	Stack<int> si;
	si.push(1);
	si.push(2);
	//si.printOn(std::cout);
	std::cout<<si<<std::endl;
}