/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/7/5 21:06:59
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <mutex>
#include <chrono>
#include <queue>
#include <memory>
#include <condition_variable>
using namespace std;
//在休眠前对互斥量进行解锁，在休眠结束后再对互斥量上锁，使另外的线程有机会获取锁并设置标识
bool flag;
std::mutex m;
void wait_for_flag(){
	std::unique_lock<std::mutex> lk(m);
	while(!flag){
		lk.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		lk.lock();
	}
}
//使用std::condition_variable处理数据等待
/*
std::queue<data_chunk> data_queue;
std::condition_variable data_cond;
void data_prepation_thread(){
	while(more_data_to_prepare()){
		const data_chunk data=prepare_data();
		std::lock_guard<std::mutex> lk(m);
		data_queue.push(data);
		data_cond.notify_one();//唤醒一个等待的线程
	}
}
void data_processing_thread(){
	while(true){
		std::unique_lock<std::mutex> lk(m);
		data_cond.wait(lk,[]{return !data_queue.empty();});//lambda表达式为等待的条件，如果为假将会解锁互斥量并将线程至于阻塞或者等待状态，如果为真则从wait返回并持有锁
		data_chunk data=data_queue.front();
		data_queue.pop();
		lk.unlock();
		process(data);
		if(is_last_chunk(data)){
			break;
		}
	}
}
*/
//线程安全的队列
template <typename T>
class threadsafe_queue{
private:
	mutable std::mutex m;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
public:
	threadsafe_queue(){}
	threadsafe_queue(const threadsafe_queue& other){
		std::lock_guard<std::mutex > lk(other.m);
		data_queue=other.data_queue;
	}
	void push(T new_value){
		std::lock_guard<std::mutex> lk(m);
		data_queue.push(new_value);
		data_cond.notify_one();
	}
	void wait_and_pop(T& value){
		std::unique_lock<std::mutex> lk(m);
		data_cond.wait(lk,[this]{return !data_queue.empty();});
		value=data_queue.front();
		data_queue.pop();
	}
	std::shared_ptr<T> wait_and_pop(){
		std::unique_lock<std::mutex> lk(m);
		data_cond.wait(lk,[this]{return !data_queue.empty();});
		std::shared_ptr<T> res=(std::make_shared<T>(data_queue.front()));
		data_queue.pop();
		return res;
	}
	bool try_pop(T& value){
		std::lock_guard<std::mutex> lk(m);
		if(data_queue.empty())return false;
		value=data_queue.front();
		data_queue.pop();
		return true;
	}
	std::shared_ptr<T> try_pop(){
		std::lock_guard<std::mutex> lk(m);
		if(data_queue.empty())return std::shared_ptr<T>();
		std::shared_ptr<T> res=std::make_shared<T>(data_queue.front());
		data_queue.pop();
		return res;
	}
	bool empty()const{//因为是const成员，所以互斥量要为可变的
		std::lock_guard<std::mutex> lk(m);
		return data_queue.empty();
	}
};
int main(){






}