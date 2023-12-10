/*************************************************************************
	> File Name: threadsafe_queue.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/12/10 19:16:06
 ************************************************************************/

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

template <typename T>
class threadsafe_queue{
private:
	struct node{
		std::shared_ptr<T> data;
		std::unique_ptr<node> next;
	};
	std::mutex head_mutex;
	std::mutex tail_mutex;
	std::unique_ptr<node> head;
	node* tail;
	std::condition_variable data_cond;
private:
	node* get_tail(){
		std::lock_guard<std::mutex> tail_lock(tail_mutex);
		return tail;
	}
	std::unique_ptr<node> pop_head(){
		std::unique_ptr<node> old_head=std::move(head);
		head=std::move(old_head->next);
		return old_head;
	}
	std::unique_lock<std::mutex> wait_for_data(){
		std::unique_lock<std::mutex> head_lock(head_mutex);
		data_cond.wait(head_lock,[&](){
			return head.get()!=get_tail();
		});
		return head_lock;
	}
	std::unique_ptr<node> wait_pop_head(){
		std::unique_lock<std::mutex> head_lock(wait_for_data());
		return pop_head();
	}
	std::unique_ptr<node> wait_pop_head(T& value){
		std::unique_lock<std::mutex> head_lock(wait_for_data());
		value=std::move(*head->data);
		return pop_head();
	}
	std::unique_ptr<node> try_pop_head(){
		std::lock_guard<std::mutex> head_lock(head_mutex);
		if(head.get()==get_tail()){
			return std::unique_ptr<node>();
		}
		return pop_head();
	}
	std::unique_ptr<node> try_pop_head(T& value){
		std::lock_guard<std::mutex> head_lock(head_mutex);
		if(head.get()==get_tail()){
			return std::unique_ptr<node>();
		}
		value=std::move(*head->data);
		return pop_head();
	}
public:
	threadsafe_queue():head(new node),tail(head.get()){}
	threadsafe_queue(const threadsafe_queue& other)=delete;
	threadsafe_queue& operator=(const threadsafe_queue& other)=delete;
	std::shared_ptr<T> wait_and_pop(){
		std::unique_ptr<node> old_head=wait_pop_head();
		return old_head->data;
	}
	void wait_and_pop(T& value){
		std::unique_ptr<node> lod_head=wait_pop_head(value);
	}
	std::shared_ptr<T> try_pop(){
		std::unique_ptr<node> old_head=try_pop_head();
		return old_head?old_head->data:std::shared_ptr<T>();
	}
	bool try_pop(T& value){
		std::unique_ptr<node> old_head=try_pop_head(value);
		return old_head;
	}
	bool empty(){
		std::lock_guard<std::mutex> head_lock(head_mutex);
		return (head.get()==get_tail());
	}
	void push(const T& new_value){
		std::shared_ptr<T> new_data(std::make_shared<T>(new_value));
		std::unique_ptr<node> p(new node);
		{
			std::lock_guard<std::mutex> tail_lock(tail_mutex);
			tail->data=new_data;
			node* new_tail=p.get();
			tail->next=std::move(p);
			tail=new_tail;
		}
		data_cond.notify_one();
	}
	void push(T&& new_value){
		std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
		std::unique_ptr<node> p(new node);
		{
			std::lock_guard<std::mutex> tail_lock(tail_mutex);
			tail->data=new_data;
			node* new_tail=p.get();
			tail->next=std::move(p);
			tail=new_tail;
		}
		data_cond.notify_one();
	}
};

#include <thread>
#include <iostream>
#include <format>
int main(){
	threadsafe_queue<int> q;
	std::thread producer1([&](){
		for(int i=0;i<100;++i){
			q.push(i);
		}
	});
	std::thread consumer1([&](){
		while(true){
			int value;
			q.wait_and_pop(value);
			std::cout<<std::format("consumer1, value is={}\n",value);
		}
	});
	std::thread consumer2([&](){
		while(true){
			auto value=q.wait_and_pop();
			std::cout<<std::format("consumer2, value is={}\n",*value);
		}
	});
	producer1.join();
	consumer1.join();
	consumer2.join();
	return 0;
}