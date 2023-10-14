/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/14 15:03:33
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <ios>

//SL.io.1 仅在必要时使用字符级输入
//SL.io.2 当进行读取时，始终要考虑非法输入的情况
void test1(){
	//只有流处于std::ios::goodbit状态时，对流的操作才会产生影响
	//当流处于std::ios::badbit状态时，它不能被重置为std::ios::goodbit
	//当std::ios::failbit置位时，需要调用clear()来重置流的状态
	//当std::ios::eofbit置位时，说明已经到达了文件尾部
	//使用stream.good()、stream.bad()、stream.fail()、stream.eof()来检查流的状态
	std::cout<<std::boolalpha<<"\n";

	std::cout<<"In failbit-state: "<<std::cin.fail()<<"\n";
	std::cout<<"\n";
	int myInt;
	while(std::cin>>myInt){
		std::cout<<"Output: "<<myInt<<"\n";
		std::cout<<"In failbit-state: "<<std::cin.fail()<<"\n";
		std::cout<<"\n";
	}
	std::cout<<"In failbit-state: "<<std::cin.fail()<<"\n";
	std::cin.clear();
	std::cout<<"In failbit-state: "<<std::cin.fail()<<"\n";
	std::cout<<"\n";
}
//SL.io.3 优先使用iostream进行I/O操作
//SL.io.10 除非你要使用printf系列函数，否则应该调用std::ios_base::sync_with_stdio(false)
//std::ios_base::sync_with_stdio(false)会使得std::cin和std::cout不再与stdio的输入输出缓冲区同步，从而提高性能
//SL.io.50 避免使用endl，使用"\n"效率更高，缓冲区会在必要的时候自动刷新

int main(){
	test1();




	return 0;
}