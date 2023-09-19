/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/19 14:33:46
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdarg>
#include <vector>
#include <thread>

//F.50 当函数不适用时(需要捕获局部变量，或者编写一个局部函数)，使用lambda表达式

void test1(){
	std::vector<std::string> vs={"15418","23212","568461"};
	std::sort(vs.begin(),vs.end(),[](const std::string& a,const std::string& b){return a.size()<b.size();});//不需要声明一个函数，这样反而会使得代码更加简洁
}

//F.52 在局部使用(包括要传递给算法)的lambda表达式中，优先通过引用捕获
//F.53 在非局部使用(包括要被返回、存储在堆上或要传递给其他线程)的lambda表达式中，避免使用引用捕获
//即使用引用捕获时，数据的生存期应该超过lambda表达式的生存期

//F.51 在有选择的情况下，优先采用默认参数而非重载
struct format{};
void print(const std::string& s,format f={}){//使用重载会需要两个函数

}

//F.55 当你的程序需要接受任意数量的参数式，不要使用va_arg参数，而是使用变参模板

int sum(int num,...){
	int sum=0;
	va_list argPointer;
	va_start(argPointer,num);
	for(int i=0;i<num;++i){
		sum+=va_arg(argPointer,int);
	}
	va_end(argPointer);
	return sum;
}

template <typename... Args>
auto tsum(Args... args){
	return (...+args);
}

void test2(){
	std::cout<<sum(3,1,2,3)<<std::endl;//6 error
	std::cout<<sum(1,2,3,3.5)<<std::endl;//2 error
	std::cout<<tsum(3,1,2,3)<<std::endl;//9
	std::cout<<tsum(1,2,3,3.5)<<std::endl;//9.5
}

int main(){
	//test1();
	test2();
	//v1
	std::string str="C++11";
	//std::thread t1([&str](){std::cout<<str<<std::endl;});//str的生存期和主线程绑定，无法保证t使用的是有效的str
	//t1.detach();
	//v2
	//std::thread t2([str](){std::cout<<str<<std::endl;});//std::cout的生存期和进程绑定，无法保证t2使用的是有效的std::cout
	//t2.detach();
	//v3
	std::thread t3([&str](){std::cout<<str<<std::endl;});
	t3.join();
}