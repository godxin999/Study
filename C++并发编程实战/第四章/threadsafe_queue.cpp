/*************************************************************************
	> File Name: threadsafe_queue.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/14 20:17:15
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <thread>
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

template <typename T>
class threadsafe_queue{
private:
	mutable std::mutex mtx;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
public:
	threadsafe_queue(){}
	threadsafe_queue(const threadsafe_queue& other){
		std::lock_guard<std::mutex> lock(other.mtx);
		data_queue=other.data_queue;
	}
	void push(T new_value){
		std::lock_guard<std::mutex> lock(mtx);
		data_queue.push(new_value);
		data_cond.notify_one();//通知条件变量有数据了
	}
	//阻塞pop
	void wait_and_pop(T& value){
		std::unique_lock<std::mutex> lock(mtx);
		data_cond.wait(lock,[this]{//使用条件变量，队列为空就阻塞
			return !data_queue.empty();
		});
		value=data_queue.front();
		data_queue.pop();
	}
	std::shared_ptr<T> wait_and_pop(){
		std::unique_lock<std::mutex> lock(mtx);
		data_cond.wait(lock,[this]{
			return !data_queue.empty();
		});
		std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
		data_queue.pop();
		return res;
	}
	//非阻塞pop
	bool try_pop(T& value){
		std::lock_guard<std::mutex> lock(mtx);
		if(data_queue.empty())return false;
		value=data_queue.front();
		data_queue.pop();
		return true;
	}
	std::shared_ptr<T> try_pop(){
		std::lock_guard<std::mutex> lock(mtx);
		if(data_queue.empty())return std::shared_ptr<T>();
		std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
		data_queue.pop();
		return res;
	}
	bool empty()const{
		std::lock_guard<std::mutex> lock(mtx);
		return queue.empty();
	}
};

void test_threadsafe_queue(){
	threadsafe_queue<int> q;
	std::mutex cout_mtx;
	std::thread producer([&](){
		for(int i=0;;++i){
			q.push(i);
			{
				std::lock_guard<std::mutex> lock(cout_mtx);
				std::cout<<"producer push data is "<<i<<std::endl;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	});

	std::thread consumer1([&](){
		for(;;){
			auto data=q.wait_and_pop();
			if(data!=nullptr){
				{
					std::lock_guard<std::mutex> lock(cout_mtx);
					std::cout<<"consumer1 wait and pop data is "<<*data<<std::endl;
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	});

	std::thread consumer2([&](){
		for(;;){
			auto data=q.wait_and_pop();
			if(data!=nullptr){
				{
					std::lock_guard<std::mutex> lock(cout_mtx);
					std::cout<<"consumer2 wait and pop data is "<<*data<<std::endl;
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	});
	producer.join();
	consumer1.join();
	consumer2.join();
}

int main(){
	test_threadsafe_queue();

	return 0;
}