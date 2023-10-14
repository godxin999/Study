/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/14 10:12:21
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <array>

//SL.con.1 优先采用STL的array或vector而不是C数组
template <typename T>
void showInfo(const T& t,const std::string& name){
	std::cout<<name<<"t.size(): "<<t.size()<<std::endl;
	std::cout<<name<<"t.capacity(): "<<t.capacity()<<std::endl;
}
void test1(){
	std::cout<<"\n";
	std::vector<int> vi;

	std::cout<<"Maximal size: "<<"\n";
	std::cout<<"vi.max_size(): "<<vi.max_size()<<"\n";
	std::cout<<"\n";

	std::cout<<"Empty vector: "<<"\n";
	showInfo(vi,"vi");
	std::cout<<"\n";

	std::cout<<"Initialized with 5 elements: "<<"\n";
	vi = {1,2,3,4,5};
	showInfo(vi,"vi");
	std::cout<<"\n";

	std::cout<<"Add four additional elements: "<<"\n";
	vi.insert(vi.end(),{6,7,8,9});
	showInfo(vi,"vi");
	std::cout<<"\n";

	std::cout<<"Resized to 30 elements: "<<"\n";
	vi.resize(30);
	showInfo(vi,"vi");
	std::cout<<"\n";

	std::cout<<"Reserved space for at least 1000 elements: "<<"\n";
	vi.reserve(1000);
	showInfo(vi,"vi");
	std::cout<<"\n";

	std::cout<<"Shrinked to the current size: "<<"\n";
	vi.shrink_to_fit();
	showInfo(vi,"vi");

	//vec.resize(n)如果n>vec.size()，那么多出的元素将会被默认初始化
	//vec.reserve(n)如果n>vec.capacity()，那么vec会获得至少能容纳n个元素的内存空间
	//vec.shrink_to_fit()将vec的capacity()缩减到size()的大小(没有强制约束力，但是总能释放不必要的内存)
}
void test2(){
	std::cout<<"\n";

	int cArr[10]={1,2,3,4,5,6,7,8,9,10};
	std::array<int,10> cppArr={1,2,3,4,5,6,7,8,9,10};
	std::vector<int> cppVec={1,2,3,4,5,6,7,8,9,10};

	std::cout<<"sizeof(cArr) = "<<sizeof(cArr)<<"\n";
	std::cout<<"sizeof(cppArr) = "<<sizeof(cppArr)<<"\n";
	std::cout<<"sizeof(cppVec) = "<<sizeof(cppVec)+cppVec.capacity()*sizeof(int)<<"\n";
	std::cout<<"               =sizeof(cppVec): "<<sizeof(cppVec)<<"\n";
	std::cout<<"               =cppVec.capacity()*sizeof(int): "<<cppVec.capacity()*sizeof(int)<<"\n";
}
//SL.con.2 默认应优先采用STL的vector，除非有理由使用别的容器
//SL.con.3 避免边界错误(STL的许多容器都支持带边界检查的at成员函数


int main(){
	//test1();
	test2();



	return 0;
}