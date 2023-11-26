/*************************************************************************
	> File Name: memory_order_acq_rel.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/25 15:18:20
 ************************************************************************/


#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <cassert>

void test_order_release_acquire(){
	std::atomic<bool> rx,ry;
	//任何指令都不能重排到acquire之前，也不能重排到release之后
	//1一定会在2之前执行
	std::thread t1([&](){
		rx.store(true,std::memory_order_relaxed);//1
		ry.store(true,std::memory_order_release);//2
	});
	//3一定会在4之后执行，而2和3之间构成同步顺序，所以1一定会在4之前执行
	std::thread t2([&]{
		while(!ry.load((std::memory_order_acquire)));//3
		assert(rx.load(std::memory_order_relaxed));//4
	});

	t1.join();
	t2.join();
}

void release_acquire_dangerous(){
	std::atomic<int> xd{0},yd{0};
	std::atomic<int> zd;

	std::thread t1([&](){
		xd.store(1,std::memory_order_release);//1
		yd.store(1,std::memory_order_release);//2
	});

	std::thread t2([&](){
		yd.store(2,std::memory_order_release);//3
	});
	//2和3只有一个可以和4构成synchronizes-with关系，当2和4构成synchronizes-with关系时，1一定会在4之前执行，当3和4构成synchronizes-with关系时，xd可能会读取到0(操作1还没执行)
	std::thread t3([&](){
		while(!yd.load(std::memory_order_acquire));//4
		assert(xd.load(std::memory_order_acquire)==1);//5
	});
	t1.join();
	t2.join();
	t3.join();
}

void release_sequence(){
	std::vector<int> v;
	std::atomic<int> flag{0};

	std::thread t1([&](){
		v.push_back(42);//1
		//以2为首的release sequence
		flag.store(1,std::memory_order_release);//2
	});

	std::thread t2([&](){
		int expected=1;
		//compare_exchange_strong(expected,2)表示如果flag的值为expected，则将flag的值改为2，返回true，否则将expected的值改为flag的值，返回false
		//3需要等待2将flag更改为1，所以1一定会在3之前执行
		while(!flag.compare_exchange_strong(expected,2,std::memory_order_relaxed)){//3
			expected=1;
		}
	});

	std::thread t3([&](){
		//4需要等待3将flag更改为2，所以1一定会在4之前执行
		while(flag.load(std::memory_order_acquire)<2);//4
		//2 synchronizes-with 4，所以断言不会触发
		assert(v.at(0)==42);//5
	});
	
	t1.join();
	t2.join();
	t3.join();

}

std::atomic<bool> x,y;
std::atomic<int> z;

void write_x(){
    x.store(true,std::memory_order_release);//1
}

void write_y(){
    y.store(true,std::memory_order_release);//2
}
//release-acquire顺序对其他线程没有影响
void read_x_then_y(){
    while(!x.load(std::memory_order_acquire));//3
    //y.load()可能看到false
	if(y.load(std::memory_order_acquire)){//4
        ++z;
	}
}

void read_y_then_x(){
    while(!y.load(std::memory_order_acquire));//5
	//x.load()可能看到false
    if(x.load(std::memory_order_acquire)){//6
        ++z;
	}
}

void release_acquire_dangerous2(){
	x=false;
	y=false;
	z=0;
	std::thread a(write_x);
	std::thread b(write_y);
	std::thread c(read_x_then_y);
	std::thread d(read_y_then_x);
	a.join();
	b.join();
	c.join();
	d.join();
	assert(z.load()!=0);//断言可能被触发
}

int main(){
	//test_order_release_acquire();
	//release_acquire_dangerous();
	//release_sequence();
	release_acquire_dangerous2();

	return 0;
}