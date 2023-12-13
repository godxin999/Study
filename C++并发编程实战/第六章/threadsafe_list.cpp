/*************************************************************************
	> File Name: threadsafe_list.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/12/12 10:29:31
 ************************************************************************/

#include <mutex>
#include <memory>
#include <cassert>
#include <iostream>
#include <format>
#include <thread>

template <typename T>
class threadsafe_list{
private:
	struct node{
		std::mutex m;
		std::shared_ptr<T> data{nullptr};
		std::unique_ptr<node> next{nullptr};
		node()=default;
		node(const T& value):data(std::make_shared<T>(value)){}
	};
	node head;
public:
	threadsafe_list()=default;
	~threadsafe_list(){
		remove_if([](const node&){
			return true;
		});
	}
	threadsafe_list(const threadsafe_list& other)=delete;
	threadsafe_list& operator=(const threadsafe_list& other)=delete;

	void push_front(const T& value){
		std::unique_ptr<node> new_node(new node(value));
		std::lock_guard<std::mutex> lk(head.m);
		new_node->next=std::move(head.next);
		head.next=std::move(new_node);
	}

	template <typename Function>
	void for_each(Function f){
		node* current=&head;
		std::unique_lock<std::mutex> lk(head.m);
		while(node* const next=current->next.get()){
			std::unique_lock<std::mutex> next_lk(next->m);
			lk.unlock();
			f(*next->data);
			current=next;
			lk=std::move(next_lk);
		}
	}

	template <typename Predicate>
	std::shared_ptr<T> find_first_if(Predicate P){
		node* current=&head;
		std::unique_lock<std::mutex> lk(head.m);
		while(node* const next=current->next.get()){
			std::unique_lock<std::mutex> next_lk(next->m);
			lk.unlock();
			if(p(*next->data)){
				return next->data;
			}
			current=next;
			lk=std::move(next_lk);
		}
		return std::shared_ptr<T>();
	}

	template <typename Predicate>
	void remove_if(Predicate p){
		node* current=&head;
		std::unique_lock<std::mutex> lk(head.m);
		while(node* const next=current->next.get()){
			std::unique_lock<std::mutex> next_lk(next->m);
			if(p(*next->data)){
				std::unique_ptr<node> old_next=std::move(current->next);
				current->next=std::move(next->next);
				next_lk.unlock();
			}
			else{
				lk.unlock();
				current=next;
				lk=std::move(next_lk);
			}
		}
	}

	template <typename Predicate>
	bool remove_first(Predicate p){
		node* current=&head;
		std::unique_lock<std::mutex> lk(head.m);
		while(node* const next=current->next.get()){
			std::unique_lock<std::mutex> next_lk(next->m);
			if(p(*next->data)){
				std::unique_ptr<node> old_next=std::move(current->next);
				current->next=std::move(next->next);
				next_lk.unlock();
				return true;
			}
			lk.unlock();
			current=next;
			lk=std::move(next_lk);
		}
		return false;
	}

	template <typename Predicate>
	void insert_if(Predicate p,const T& value){
		node* current=&head;
		std::unique_lock<std::mutex> lk(head.m);
		while(node* const next=current->next.get()){
			std::unique_lock<std::mutex> next_lk(next->m);
			if(p(*next->data)){
				std::unique_ptr<node> new_node(new node(value));
				auto old_next=std::move(current->next);
				new_node->next=std::move(old_next);
				current->next=std::move(new_node);
				return;
			}
			lk.unlock();
			current=next;
			lk=std::move(next_lk);
		}
	}
};

int main(){
	std::cout<<"begin\n";
	threadsafe_list<int> list;
	std::thread t1([&list]{
		for(int i=0;i<1000;++i){
			list.push_front(i);
		}
		std::cout<<"t1 end\n";
	});
	std::thread t2([&list]{
		for(int i=0;i<1000;++i){
			list.remove_first([i](int value){
				return value==i;
			});
		}
		std::cout<<"t2 end\n";
	});
	std::thread t3([&list]{
		for(int i=0;i<1000;++i){
			list.remove_if([i](int value){
				return value==i;
			});
		}
		std::cout<<"t3 end\n";
	});
	t1.join();
	t2.join();
	t3.join();
	list.for_each([](int value){
		std::cout<<value<<std::endl;
	});
	std::cout<<"end\n";
	return 0;
}