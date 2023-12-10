/*************************************************************************
	> File Name: threadsafe_stack.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/12/10 20:26:15
 ************************************************************************/

#include <stack>
#include <memory>
#include <mutex>
#include <condition_variable>

template <typename T>
class threadsafe_stack{
private:
	std::stack<std::shared_ptr<T>> data;
	std::mutex mtx;
	std::condition_variable data_cond;
public:
	threadsafe_stack(){}
	threadsafe_stack(const threadsafe_stack& other){
		std::lock_guard<std::mutex> lk(other.mtx);
		data=other.data;
	}
	threadsafe_stack& operator=(const threadsafe_stack&)=delete;
	void push(const T& value){
		std::lock_guard<std::mutex> lk(mtx);
		data.push(std::make_shared<T>(value));
		data_cond.notify_one();
	}
	void push(T&& value){
		std::lock_guard<std::mutex> lk(mtx);
		data.push(std::make_shared<T>(std::move(value)));
		data_cond.notify_one();
	}
	std::shared_ptr<T> wait_and_pop(){
		std::unique_lock<std::mutex> lk(mtx);
		data_cond.wait(lk,[&](){
			return !data.empty();
		});
		std::shared_ptr<T> res=data.top();
		data.pop();
		return res;
	}
	void wait_and_pop(T& value){
		std::unique_lock<std::mutex> lk(mtx);
		data_cond.wait(lk,[&](){
			return !data.empty();
		});
		value=std::move(*data.top());
		data.pop();
	}
	std::shared_ptr<T> try_pop(){
		std::lock_guard<std::mutex> lk(mtx);
		if(data.empty()){
			return std::shared_ptr<T>();
		}
		std::shared_ptr<T> res=data.top();
		data.pop();
		return res;
	}
	bool try_pop(T& value){
		std::lock_guard<std::mutex> lk(mtx);
		if(data.empty()){
			return false;
		}
		value=std::move(*data.top());
		data.pop();
		return true;
	}
	bool empty(){
		std::lock_guard<std::mutex> lk(mtx);
		return data.empty();
	
	}
};


#include <thread>
#include <iostream>
#include <format>
int main(){
	threadsafe_stack<int> s;
	std::thread producer1([&](){
		for(int i=0;i<100;++i){
			s.push(i);
			//std::cout<<std::format("producer1, value is={}\n",i);
		}
	});
	std::thread consumer1([&](){
		while(true){
			int value;
			s.wait_and_pop(value);
			std::cout<<std::format("consumer1, value is={}\n",value);
		}
	});
	std::thread consumer2([&](){
		while(true){
			auto value=s.wait_and_pop();
			std::cout<<std::format("consumer2, value is={}\n",*value);
		}
	});
	producer1.join();
	consumer1.join();
	consumer2.join();
	return 0;
}