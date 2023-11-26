/*************************************************************************
	> File Name: memory_order_consume.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/26 13:43:53
 ************************************************************************/

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <cassert>

//memory_order_consume可以用于load操作，使用memory_order_consume的load称为consume操作，如果一个consume操作在同一个原子变量上读到了一个release操作写入的值，或以其为首的release sequence写入的值，则称这个release操作dependency-ordered before这个consume操作，且只针对这个原子变量有效

void test_order_consume(){
	std::atomic<std::string*> ptr;
	int data=0;
	std::thread t1([&](){
		std::string* p=new std::string("Hello");//1
		data=42;//2
		ptr.store(p,std::memory_order_release);//3
	});

	std::thread t2([&](){
		std::string* p2;
		//因为4依赖于3执行完，所以3 dependence-ordered before 4，或者3 inter-thread happens-before 4
		while(!(p2=ptr.load(std::memory_order_consume)));//4
		//1 happens-before 5，所以5不会被触发
		assert(*p2=="Hello");//5
		//因为data不和3构成dependency-ordered关系，所以data可能读取到0
		//如果将4改为memory_order_acquire，那么2 happens-before 6，6不会被触发
		assert(data==42);//6
	});

	t1.join();
	t2.join();
}


int main(){
	test_order_consume();


	return 0;
}