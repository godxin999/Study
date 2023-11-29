/*************************************************************************
	> File Name: queue_atomic.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/29 17:51:02
 ************************************************************************/

#include <iostream>
#include <format>
#include <mutex>
#include <queue>
#include <memory>
#include <thread>

template <typename T,size_t Cap>
class QueueAtomic:private std::allocator<T>{
public:
	QueueAtomic():max_size(Cap+1),data(std::allocator<T>::allocate(max_size)),head(0),tail(0),tail_update(0){}
	QueueAtomic(const QueueAtomic&)=delete;
	QueueAtomic& operator=(const QueueAtomic&)=delete;
	QueueAtomic& operator=(const QueueAtomic&)volatile=delete;
	~QueueAtomic(){
		while(head!=tail){
			std::destroy_at(data+head);
			head=(head+1)%max_size;
			std::cout<<"destroy\n";
		}
		std::allocator<T>::deallocate(data,max_size);
	}

	bool pop(T& val){
		size_t h;
		do{
			h=head.load(std::memory_order_relaxed);
			if(h==tail.load(std::memory_order_acquire)){
				std::cout<<"queue is empty\n";
				return false;
			}
			if(h==tail_update.load(std::memory_order_acquire)){
				return false;
			}
			val=data[h];
		}
		while(!head.compare_exchange_strong(h,(h+1)%max_size,std::memory_order_release,std::memory_order_relaxed));
		return true;
	}

	bool push(const T& val){
		size_t t;
		do{
			t=tail.load(std::memory_order_relaxed);
			if((t+1)%max_size==head.load(std::memory_order_acquire)){
				std::cout<<"queue is full\n";
				return false;
			}
			//std::cout<<t<<std::endl;
		}
		while(!tail.compare_exchange_strong(t,(t+1)%max_size,std::memory_order_release,std::memory_order_relaxed));
		data[t]=val;
		//std::cout<<t<<std::endl;
		size_t tailup;
		do{
			tailup=t;
		}
		while(!tail_update.compare_exchange_strong(tailup,(tailup+1)%max_size,std::memory_order_release,std::memory_order_relaxed));
		std::cout<<std::format("{} {} {}\n",head.load(),tail.load(),tail_update.load());
		return true;
	}

private:
	size_t max_size;
	T* data;
	std::atomic<size_t> head;
	std::atomic<size_t> tail;
	std::atomic<size_t> tail_update;
};


int main(){
	QueueAtomic<int,200> q;
	std::jthread t1([&]{
		for(int i=0;i<10;++i){
			q.push(i);
		}
	});
	
	std::jthread t2([&]{
		for(int i=0;i<10;++i){
			q.push(i);
		}
	});
	
	std::jthread t3([&]{
		for(int i=0;i<10;++i){
			q.push(i);
		}
	});

	std::jthread t4([&]{
		for(int i=0;i<10;++i){
			int val;
			q.pop(val);
		}
	});

	std::jthread t5([&]{
		for(int i=0;i<10;++i){
			q.push(i);
		}
	});
	
	std::jthread t6([&]{
		for(int i=0;i<10;++i){
			q.push(i);
		}
	});
	
	std::jthread t7([&]{
		for(int i=0;i<10;++i){
			q.push(i);
		}
	});

	std::jthread t8([&]{
		for(int i=0;i<10;++i){
			int val;
			q.pop(val);
		}
	});

	std::jthread t10([&]{
		for(int i=0;i<10;++i){
			q.push(i);
		}
	});
	
	std::jthread t12([&]{
		for(int i=0;i<10;++i){
			q.push(i);
		}
	});
	
	std::jthread t13([&]{
		for(int i=0;i<10;++i){
			q.push(i);
		}
	});

	std::jthread t14([&]{
		for(int i=0;i<10;++i){
			int val;
			q.pop(val);
		}
	});

	std::jthread t15([&]{
		for(int i=0;i<10;++i){
			q.push(i);
		}
	});
	
	std::jthread t16([&]{
		for(int i=0;i<10;++i){
			q.push(i);
		}
	});
	
	std::jthread t17([&]{
		for(int i=0;i<10;++i){
			q.push(i);
		}
	});

	std::jthread t18([&]{
		for(int i=0;i<10;++i){
			int val;
			q.pop(val);
		}
	});
	

	return 0;
}
