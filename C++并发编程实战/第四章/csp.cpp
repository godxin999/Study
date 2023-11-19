/*************************************************************************
	> File Name: csp.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/19 15:05:54
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <future>
#include <chrono>
#include <format>
#include <queue>


template <typename T>
class Channel{
private:
	std::queue<T> queue_;
	std::mutex mtx_;
	std::condition_variable cv_producer_;
	std::condition_variable cv_consumer_;
	size_t capacity_;
	bool closed_=false;

public:
	Channel(size_t capacity=0):capacity_(capacity){}

	bool send(T value){
		std::unique_lock<std::mutex> lock(mtx_);
		cv_producer_.wait(lock,[this](){
			//开启状态时，如果有缓冲区，缓冲区满后挂起
			//capacity_为零表示是一个无缓冲区的Channel，需要等待有消费者准备好
			return capacity_==0&&queue_.empty()||queue_.size()<capacity_||closed_;
		});

		if(closed_){
			return false;
		}

		queue_.push(value);
		cv_consumer_.notify_one();
		return true;
	}

	bool receive(T& value){
		std::unique_lock<std::mutex> lock(mtx_);
		cv_consumer_.wait(lock,[this](){
			return !queue_.empty()||closed_;
		});

		if(closed_&&queue_.empty()){
			return false;
		}

		value=queue_.front();
		queue_.pop();
		cv_producer_.notify_one();
		return true;
	}

	void close(){
		std::unique_lock<std::mutex> lock(mtx_);
		closed_=true;
		cv_producer_.notify_all();
		cv_consumer_.notify_all();
	}
};

int main(){

	Channel<int> channel(10);

	std::thread producer([&](){
		for(int i=0;i<3;++i){
			channel.send(i);
			std::cout<<std::format("send {}\n",i);
		}
		channel.close();
	});

	std::thread consumer([&](){
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		int value;
		while(channel.receive(value)){
			std::cout<<std::format("receive {}\n",value);
		}
	});

	producer.join();
	consumer.join();

	return 0;
}