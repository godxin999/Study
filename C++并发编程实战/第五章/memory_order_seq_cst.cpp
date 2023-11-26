/*************************************************************************
	> File Name: memory_order_seq_cst.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/25 15:18:20
 ************************************************************************/


#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <cassert>

std::atomic<bool> x,y;
std::atomic<int> z;

void write_x_then_y(){
	//std::memory_seq_cst内存序，具有全局一致性
	x.store(true,std::memory_order_seq_cst);
	y.store(true,std::memory_order_seq_cst);
}

void read_y_then_x(){
	while(!y.load(std::memory_order_seq_cst)){
		std::cout<<"y load false\n";
	}
	if(x.load(std::memory_order_seq_cst)){
		++z;
	}
	
}

void test_order_seq_cst(){
	x=false;
	y=false;
	z=0;

	std::thread t1(write_x_then_y);
	std::thread t2(read_y_then_x);

	t1.join();
	t2.join();

	assert(z.load()!=0);
}

void test_order_seq_cst2(){
	std::atomic<bool> rx,ry;

	std::thread t1([&](){
		rx.store(true,std::memory_order_seq_cst);
		ry.store(true,std::memory_order_seq_cst);
	});

	std::thread t2([&]{
		while(!ry.load((std::memory_order_seq_cst)));
		assert(rx.load(std::memory_order_seq_cst));
	});

	t1.join();
	t2.join();
}

int main(){
	//test_order_seq_cst();
	test_order_seq_cst2();



	return 0;
}