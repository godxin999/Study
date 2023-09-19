/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/18 22:38:44
 ************************************************************************/

#include <iostream>
#include <vector>
#include "stack.hpp"



int main(){
	Stack<int> Si;
	Stack<float> Sf;
	Si.push(1);
	Si.push(2);
	std::cout<<Si.top()<<std::endl;
	Sf.push(2.2);
	Sf.push(3.3);
	std::cout<<Sf.top()<<std::endl;
	Stack<double,std::vector> Sdv;
	Sdv.push(4.4);
	Sdv.push(5.5);
	std::cout<<Sdv.top()<<std::endl;
	Sdv=Sf;
	while(!Sdv.empty()){
		std::cout<<Sdv.top()<<std::endl;
		Sdv.pop();
	}
	std::cout<<std::endl;



	return 0;
}