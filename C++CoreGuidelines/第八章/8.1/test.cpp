/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/3 9:27:58
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <utility>
#include <numeric>


//表达式的结果为值，如5*5
//语句用于做某事，如std::cout << "Hello, world!" << std::endl;

//ES.1 优先使用标准库，而不是其他库或手工代码
void test1(){
	std::vector<int> v{1,2,3,4,5};
	/*int max=v.size();
	double sum=0.0;
	for(int i=0;i<max;i++){
		sum+=v[i];
	}*/
	auto sum=std::accumulate(v.begin(),v.end(),0.0);//使用标准库函数
	//auto pro=std::accumulate(v.begin(),v.end(),1.0,[](double f,double s){return f*s;});
	auto pro=std::accumulate(v.begin(),v.end(),1.0,std::multiplies<double>());//使用标准库函数
}
//ES.2 优先使用合适的抽象，而非直接使用语言特性

int main(){
	test1();
	return 0;
}