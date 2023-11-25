/*************************************************************************
	> File Name: memory_order_relaxed.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/24 16:53:50
 ************************************************************************/

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <cassert>

//宽松内存次序(std::memory_order_relaxed)
//1.作用于原子变量
//2.不具有synchronizes-with(happens-before)关系(简单来说如果多线程环境下，有一个线程先修改了变量m，我们将这个操作叫做A，之后有另一个线程读取变量m，我们将这个操作叫做B，那么B一定读取A修改m之后的最新值，即A happens-before B)
//3.对于同一个原子变量，在同一个线程中具有happens-before关系，在同一线程中的不同的原子变量不具有happens-before关系，可能被重排
//4.多线程情况下不具有happens-before关系

std::atomic<bool> x,y;
std::atomic<int> z;

void write_x_then_y(){
	x.store(true,std::memory_order_relaxed);
	y.store(true,std::memory_order_relaxed);
}

void read_y_then_x(){
	while(!y.load(std::memory_order_relaxed)){
		std::cout<<"y load false\n";
	}
	if(x.load(std::memory_order_relaxed)){
		++z;
	}

}

void test_order_relaxed(){
	x=false;
	y=false;
	z=0;

	std::thread t1(write_x_then_y);
	std::thread t2(read_y_then_x);

	t1.join();
	t2.join();
	assert(z.load()!=0);//断言可能会触发
}

void test_order_relaxed2(){
	std::atomic<int> a{0};
	std::vector<int> v1,v2;
	std::thread t1([&a](){
		for(int i=0;i<10;i+=2){
			a.store(i,std::memory_order_relaxed);
		}
	});

	std::thread t2([&a](){
		for(int i=1;i<10;i+=2){
			a.store(i,std::memory_order_relaxed);
		}
	});
	//多线程对同一个原子变量进行修改，std::memory_order_relaxed保证对a的操作是原子的，如果t3读到的序列包含5、6、7，那么t4读到的序列一定不会出现5、7、6
	std::thread t3([&a,&v1](){
		for(int i=0;i<10;++i){
			v1.push_back(a.load(std::memory_order_relaxed));
		}
	});

	std::thread t4([&a,&v2](){
		for(int i=0;i<10;++i){
			v2.push_back(a.load(std::memory_order_relaxed));
		}
	});

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	for(const auto& i:v1){
		std::cout<<i<<" ";
	}
	std::cout<<"\n";
	for(const auto& i:v2){
		std::cout<<i<<" ";
	}
	std::cout<<"\n";
}

int main(){
	test_order_relaxed();
	//test_order_relaxed2();



	return 0;
}