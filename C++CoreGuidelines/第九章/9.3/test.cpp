/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/10 10:18:12
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <chrono>
using namespace std::chrono;

//对于自赋值检查，如果类成员是STL成员、std::string或内置类型，则不需要自赋值检查，如果成员包括指针(涉及内存分配)，则需要进行自赋值检查
//可以为类定义一个swap函数并借此实现拷贝构造函数，如此不需要进行自赋值检查

//Per.10 依赖静态类型系统
//1.写本地代码(使用lambda代替函数)
bool lessLength(const std::string& f,const std::string& s){
	return f.size()<s.size();
}
void test1(){
	std::vector<std::string> vs1{"12345","123456","1234","1","12","123","12345"};
	auto vs2=vs1;
	auto start=high_resolution_clock::now();
	std::sort(vs1.begin(),vs1.end(),lessLength);
	auto end=high_resolution_clock::now();
	std::cout<<duration_cast<nanoseconds>(end-start).count()<<std::endl;
	start=high_resolution_clock::now();
	std::sort(vs2.begin(),vs2.end(),[](const std::string& f,const std::string& s){return f.size()<s.size();});
	end=high_resolution_clock::now();
	std::cout<<duration_cast<nanoseconds>(end-start).count()<<std::endl;
}
//2.写简单代码，复杂的代码会导致优化编译器不充分
//3.给予编译器额外的提示，如noexcept、final等

//Per.11 将计算从运行期移至编译期
//Per.19 以可预测的方式访问内存(优先使用std::array、std::vector、std::string)


int main(){
	test1();




	return 0;
}