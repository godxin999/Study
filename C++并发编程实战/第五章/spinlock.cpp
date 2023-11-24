/*************************************************************************
	> File Name: spinlock.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/24 15:25:10
 ************************************************************************/

#include <iostream>
#include <atomic>
#include <thread>

//自旋锁
class Spinlock{
public:
	void lock(){
		while(flag.test_and_set(std::memory_order_acquire));//自旋等待，直到成功获取到锁
	}
	void unlock(){
		flag.clear(std::memory_order_release);//释放锁
	}
private:
	std::atomic_flag flag=ATOMIC_FLAG_INIT;
};

int main(){
	Spinlock spin;
	std::jthread t1([&spin](){
		spin.lock();
		for(int i=0;i<3;++i){
			std::cout<<"t1\n";
		}
		spin.unlock();
	});

	std::jthread t2([&spin](){
		spin.lock();
		for(int i=0;i<3;++i){
			std::cout<<"t2\n";
		}
		spin.unlock();
	});

	return 0;
}