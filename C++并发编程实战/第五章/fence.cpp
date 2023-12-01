/*************************************************************************
	> File Name: fence.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/12/1 21:34:00
 ************************************************************************/

#include <iostream>
#include <thread>
#include <atomic>
#include <cassert>

std::atomic<bool> x,y;
std::atomic<int> z;

//atomic_thread_fence和release-acquire相似，都是保证memory_order_release之前的指令不会排到其后，memory_order_acquire之后的指令不会排到其之前

void write_x_then_y_fence(){
	x.store(true,std::memory_order_relaxed);
	std::atomic_thread_fence(std::memory_order_release);
	y.store(true,std::memory_order_relaxed);
}

void read_y_then_x_fence(){
	while(!y.load(std::memory_order_relaxed));
	std::atomic_thread_fence(std::memory_order_acquire);
	if(x.load(std::memory_order_relaxed)){
		++z;
	}
}


int main(){
	x=false;
	y=false;
	z=0;
	std::thread a(write_x_then_y_fence);
	std::thread b(read_y_then_x_fence);
	a.join();
	b.join();
	assert(z.load()!=0);
	return 0;
}