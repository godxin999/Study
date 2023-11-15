/*************************************************************************
	> File Name: test_thread_pool.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/15 23:06:59
 ************************************************************************/

#include <iostream>
#include "thread_pool.h"


int main(){
/*
	int m=0;
	ThreadPool::instance().commit([](int &m){
		m=1024;
		std::cout<<std::format("inner set m is {}\n",m);
		//std::cout<<std::format("m address is {:p}\n",(void*)&m);
	},std::ref(m));

	std::this_thread::sleep_for(std::chrono::seconds(3));
	std::cout<<std::format("m is {}\n",m);
	//std::cout<<std::format("m address is {:p}\n",(void*)&m);
*/

	auto future=ThreadPool::instance().commit([]()->int{
		throw std::runtime_error("error");
		return 1;
	});

	std::this_thread::sleep_for(std::chrono::seconds(3));

	try{
		future.get();
	}
	catch(const std::exception& e){
		std::cout<<e.what()<<std::endl;
	}
	return 0;
}