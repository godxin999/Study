/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/2 20:06:35
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <thread>
#include <vector>
#include <exception>
#include <memory>
#include <stack>
#include <list>
#include <mutex>
using namespace std;
//使用互斥量mutex保护链表
std::list<int> some_list;
std::mutex some_mutex;
void add_to_list(int new_value){
	//std::lock_guard<std::mutex> guard(some_mutex);
	//std::lock_guard guard(some_mutex);//C++17可以省略lock_guard的模板参数列表
	std::scoped_lock guard(some_mutex);//C++17中的加强版数据保护机制
	some_list.push_back(new_value);
}
bool list_contains(int value_to_find){
	//std::lock_guard<std::mutex> guard(some_mutex);//lock_guard使这两个函数中对数据的访问是互斥的
	std::lock_guard guard(some_mutex);
	return std::find(some_list.begin(),some_list.end(),value_to_find)!=some_list.end();
}

class some_data{
	int a;
	string b;
public:
	void do_something(){}
};
class data_wrapper{
private:
	some_data data;
	std::mutex m;
public:
	template<typename Function>
	void process_data(Function func){//看起来没有问题，lock_guard对数据进行了保护，但是调用用户提供的函数func，就意味着foo能够绕过保护将函数malicious_function传进去，可以在没有锁定互斥量的情况下调用do_something()
		std::lock_guard l(m);
		func(data);
	}
};
some_data* unprotected;
void malicious_function(some_data &protected_data){
	unprotected=&protected_data;//切勿将受保护的数据的指针或引用传递到互斥锁作用域之外
}
data_wrapper x;
void foo(){
	x.process_data(malicious_function);
	unprotected->do_something();
}
//解决接口间的条件竞争
//选项一:传入一个引用
void option1(){
	vector<int> result;
	//some_stack.pop(result);//传入一个引用以接收弹出值
	//对于一些类型，临时构造一个一个实例是不划算的，而且需要可赋值的存储类型
}
//选项二:使用无异常抛出的构造函数或移动构造函数
//安全但不可靠，存在一些抛出异常的构造函数但是却没有移动构造函数的类型
//选项三:返回指向弹出值的指针
//需要对对象的内存分配进行管理，虽然可以使用shared_ptr，但是开销较大
//选项四:选项一+选项二或选项一+选项三
//定义线程安全的堆栈(选项一+选项三)
struct empty_stack:std::exception{
	const char* what()const throw(){//当栈为空时，pop()函数会抛出empty_stack异常
		return "empty stack!";
	}
};
template <typename T>
class threadsafe_stack{
private:
	std::stack<T> data;
	mutable std::mutex m;
public:
	threadsafe_stack():data(std::stack<T>()){}
	threadsafe_stack(const threadsafe_stack &other){
		std::lock_guard<std::mutex> lock(other.m);//防止拷贝时other对象被其他线程修改
		data=other.data;
	}
	threadsafe_stack& operator=(const threadsafe_stack&)=delete;
	//通过削减接口可以获得最大程度的安全
	void push(T new_value){
		std::lock_guard<std::mutex> lock(m);
		data.push(new_value);
	}
	std::shared_ptr<T> pop(){//使用shared_ptr避免内存管理的问题
		std::lock_guard<std::mutex> lock(m);
		if(data.empty())throw empty_stack();
		const std::shared_ptr<T> res(std::make_shared<T>(data.top()));//创建指向栈顶元素副本的shared_ptr
		data.pop();
		return res;
	}
	void pop(T& value){
		std::lock_guard<std::mutex> lock(m);
		if(data.empty())throw empty_stack();
		value=data.top();//将元素存储到引用中
		data.pop();
	}
	bool empty()const{
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
};
//使用lock一次性锁住多个互斥量
class some_big_object{};
void swap(some_big_object& lhs,some_big_object& rhs);
class X{
private:
	some_big_object some_detail;
	std::mutex m;
public:
	X(const some_big_object& sd):some_detail(sd){}
	friend void swap(X& lhs,X& rhs){
		if(&lhs==&rhs)return;
		std::lock(lhs.m,rhs.m);//std::lock锁住两个互斥量
		//当std::lock获取了一个互斥锁后，再尝试从另一个互斥量上获取锁时，就会抛出异常，第一个锁会自动释放，所以std::lock要么将两个锁都锁住，要么一个都不锁
		std::lock_guard<std::mutex> lock_a(lhs.m,std::adopt_lock);//std::adopt_lock将锁交由std::lock_guard管理
		std::lock_guard<std::mutex> lock_b(rhs.m,std::adopt_lock);
		swap(lhs.some_detail,rhs.some_detail);
	}
	//C++17std::scoped_lock可以接收不定数量的互斥量类型作为模板参数，以及相应的互斥量作为构造参数
	void swap2(X& lhs,X& rhs){
		if(&lhs==&rhs)return;
		std::scoped_lock guard(lhs.m,rhs.m);//用法和std::lock相同，互斥量在构造时上锁，在析构时解锁
		swap(lhs.some_detail,rhs.some_detail);
	}
};



int main(){






}