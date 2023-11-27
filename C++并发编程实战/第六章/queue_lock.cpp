/*************************************************************************
	> File Name: queue_lock.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/27 10:07:19
 ************************************************************************/

#include <iostream>
#include <format>
#include <mutex>
#include <queue>
#include <memory>

template <typename T,size_t Cap>
class QueueLock:private std::allocator<T>{
public:
	QueueLock():max_size(Cap+1),data(std::allocator<T>::allocate(max_size)),head(0),tail(0){}
	QueueLock(const QueueLock&)=delete;
	QueueLock& operator=(const QueueLock&)=delete;
	QueueLock& operator=(const QueueLock&)volatile=delete;
	~QueueLock(){
		std::lock_guard<std::mutex> lock(mtx);
		while(head!=tail){
			std::allocator<T>::destroy(data+head);
			head=(head+1)%max_size;
		}
		std::allocator<T>::deallocate(data,max_size);
	}

	template <typename... Args>
	bool emplace(Args&&... args){
		std::lock_guard<std::mutex> lock(mtx);
		if((tail+1)%max_size==head){
			std::cout<<"queue is full\n";
			return false;
		}
		std::allocator<T>::construct(data+tail,std::forward<Args>(args)...);
		tail=(tail+1)%max_size;
		return true;
	}

	bool push(const T& value){
		std::cout<<"push const T& value\n";
		return emplace(value);
	}

	bool push(T&& value){
		std::cout<<"push T&& value\n";
		return emplace(std::move(value));
	}

	bool pop(T& value){
		std::lock_guard<std::mutex> lock(mtx);
		if(head==tail){
			std::cout<<"queue is empty\n";
			return false;
		}
		value=std::move(data[head]);
		head=(head+1)%max_size;
		return true;
	}

private:
	size_t max_size;
	T* data;
	std::mutex mtx;
	size_t head;
	size_t tail;
};

class A{
public:
	A(int a,int b):a(a),b(b){}
	A(std::initializer_list<int> list){
		auto it=list.begin();
		a=*it;
		b=*(++it);
	}
	int a;
	int b;
};

#include <string>

class B{
public:
	B(int a,std::string b):a(a),b(b){}
	int a;
	std::string b;
};


int main(){
	QueueLock<B,10> q;
	/*
	q.push(1);
	q.push(2);
	for(int i=0;i<10;i++){
		if(!q.push(i))break;
	}
	for(int i=0;i<12;i++){
		int val;
		if(q.pop(val)){
			std::cout<<val<<std::endl;
		}
	}
	*/
	/*
	q.push(A(1,2));
	q.emplace(3,4);
	q.push({5,6});
	q.emplace(std::initializer_list<int>{5,6});
	*/
	q.push(B{1,"hello"});
	q.emplace(2,"world");
	q.push({3,"111"});
	return 0;
}