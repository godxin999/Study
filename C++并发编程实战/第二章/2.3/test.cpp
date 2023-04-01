/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/1 23:12:54
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <thread>
#include <vector>
#include <functional>
using namespace std;
//转移线程的所有权
void some_func(){}
void some_other_func(){}
void test1(){
	thread t1(some_func);
	thread t2=std::move(t1);//转移t1的所有权到t2
	t1=thread(some_other_func);//因为所有权在一个临时对象上，所以std::move会隐式调用
	thread t3;//使用默认构造方式创建t3
	t3=std::move(t2);//将t2的所有权转移到t3上
	t1=std::move(t3);//因为t1已经有了关联的线程，这里系统会调用terminate()函数终止程序
}
//函数返回thread对象
thread f(){
	return thread(some_func);
}
thread g(){
	thread t(some_other_func);
	return t;
}
//thread作为参数传递
void f(thread t){}
void test2(){
	f(thread(some_func));
	thread t(some_func);
	f(std::move(t));
}
//确保线程在程序推出前完成
class scoped_thread{
	thread t;
public:
	explicit scoped_thread(thread t_):t(std::move(t_)){
		if(!t.joinable()){
			throw std::logic_error("No thread");
		}
	}
	~scoped_thread(){
		t.join();
	}
	scoped_thread(const scoped_thread&)=delete;
	scoped_thread& operator=(const scoped_thread&)=delete;
};
void do_something_in_current_thread(){}
void do_something(int){}
struct func{
	int &i;
	func(int &i_):i(i_){}
	void operator()(){
		cout<<"thread open"<<endl;
		for(unsigned j=0;j<1e6;++j){
			do_something(i);//存在访问隐患，悬空引用
		}
	}
};
void test3(){
	int some_local_state=0;
	scoped_thread t((thread(func(some_local_state))));//scoped_thread类对象在析构时会加入(join)到构造函数创建的线程中去
	do_something_in_current_thread();
}
//joining_thread
class joining_thread{
	thread t;
public:
	joining_thread()noexcept=default;
	template<typename Callable,typename ... Args>
	explicit joining_thread(Callable &&func,Args&& ... args):t(std::forward<Callable>(func),std::forward<Args>(args)...){}
	explicit joining_thread(thread t_)noexcept:t(std::move(t_)){}
	joining_thread(joining_thread &&other)noexcept:t(std::move(other.t)){}
	joining_thread& operator=(joining_thread &&other)noexcept{
		if(joinable()){
			join();
		}
		t=std::move(other.t);
		return *this;
	}
	joining_thread& operator=(thread other)noexcept{
		if(joinable()){
			join();
		}
		t=std::move(other);
		return *this;
	}
	~joining_thread()noexcept{
		if(joinable()){
			join();
		}
	}
	void swap(joining_thread &other)noexcept{
		t.swap(other.t);
	}
	thread::id get_id()const noexcept{
		return t.get_id();
	}
	bool joinable()const noexcept{
		return t.joinable();
	}
	void join(){
		t.join();
	}
	void detach(){
		t.detach();
	}
	thread& as_thread()noexcept{
		return t;
	}
	const thread& as_thread()const noexcept{
		return t;
	}
};
//量产线程并等待它们结束
void do_work(unsigned id){}
void test4(){
	vector<thread> threads;
	for(auto i=0;i<20;++i){
		threads.push_back(thread(do_work,i));
	}
	for_each(threads.begin(),threads.end(),std::mem_fn(&thread::join));//对每个线程调用join
}
int main(){
	//test1();
	//test2();
	//test3();
	test4();
}