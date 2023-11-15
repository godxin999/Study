/*************************************************************************
	> File Name: async.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/15 19:27:05
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <thread>
#include <future>
#include <format>

std::string fetchDataFromDB(std::string query){
	std::this_thread::sleep_for(std::chrono::seconds(5));
	return "Data:"+query;
}
void use_async(){
	//使用std::async异步调用fetchDataFromDB函数
	//std::launch::deferred表示延迟调用，调用get()或者wait()函数时才会执行
	//默认情况下std::async采用的是std::launch::async|std::launch::deferred策略，具体采用哪种策略取决于系统
	std::future<std::string> resultFromDB=std::async(std::launch::async,fetchDataFromDB,"Data");
	//在主线程中做其他事情
	std::cout<<"do something else..."<<std::endl;
	//从future对象中获取数据
	std::string dbData=resultFromDB.get();//阻塞操作，只有当子线程中的函数执行完毕后才会继续执行主线程
	std::cout<<dbData<<std::endl;
}
//std::future::get()是一个阻塞调用，如果异步任务还没有完成，那么get()函数会阻塞当前线程，直到任务完成，如果任务完成，get()函数会立刻返回任务的结果
//get()只能调用一次，因为它会消耗掉std::future的状态
//std::future::wait()是一个阻塞调用，它不会返回任务的结果，知识等待异步任务结束，如果任务还没有结束，那么wait()函数会阻塞当前线程，直到任务完成
//wait()可以被多次调用，它不会消耗掉std::future的状态
//可以使用std::wait_for()和std::wait_until()函数来检查异步操作是否已经完成，这两个函数返回一个std::future_status枚举值，表示异步操作的状态
void test_wait_for(){
	std::future<std::string> resultFromDB=std::async(std::launch::async,fetchDataFromDB,"Data");
	std::cout<<"do something else..."<<std::endl;
	//检查异步操作是否已经完成
	std::future_status status;
	do{
		status=resultFromDB.wait_for(std::chrono::seconds(1));
		if(status==std::future_status::deferred){
			std::cout<<"deferred"<<std::endl;
		}else if(status==std::future_status::timeout){
			std::cout<<"timeout"<<std::endl;
		}else if(status==std::future_status::ready){
			std::cout<<"ready"<<std::endl;
		}
	}while(status!=std::future_status::ready);
	std::string dbData=resultFromDB.get();
	std::cout<<dbData<<std::endl;
}

int my_task(){
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::cout<<"my task run 5 s"<<std::endl;
	return 42;
}

void use_package(){
	//创建一个包装了任务的std::packaged_task对象
	std::packaged_task<int()> task(my_task);
	//调用get_future()函数来获取与任务关联的std::future对象
	std::future<int> result=task.get_future();
	//在另一个线程上执行任务
	std::thread t(std::move(task));
	t.detach();//线程分离，以便主线程可以等待任务完成
	//等待任务完成并获取结果
	int value=result.get();
	std::cout<<"Value : "<<value<<std::endl;
}

void set_value(std::promise<int> prom){
	std::this_thread::sleep_for(std::chrono::seconds(5));
	prom.set_value(42);
	std::cout<<"promise set value success\n";
}

void use_promise(){
	std::promise<int> prom;
	std::future<int> result=prom.get_future();
	std::thread t(set_value,std::move(prom));
	std::cout<<"waiting for the thread to set the value\n";
	std::cout<<std::format("value set by the thread:{}\n",result.get());
	t.join();
}

void set_exception(std::promise<void> prom){
	try{
		throw std::runtime_error("An error occurred!");
	}
	catch(...){
		prom.set_exception(std::current_exception());
	}
}

void use_promise_exception(){
	std::promise<void> prom;
	auto result=prom.get_future();
	std::thread t(set_exception,std::move(prom));//将prom转移给线程t，线程t负责设置共享状态的值来通知主线程
	try{
		std::cout<<"Waiting for the thread to set the exception\n";
		result.get();
	}
	catch(const std::exception& e){
		std::cout<<std::format("Exception set by the thread: {}\n",e.what());
	}
	t.join();
}

void my_func(std::promise<int>&& prom){
	std::this_thread::sleep_for(std::chrono::seconds(5));
	prom.set_value(42);
}

void thread_func(std::shared_future<int> future){
	try{
		int result=future.get();
		std::cout<<std::format("result: {}\n",result);
	}
	catch(const std::future_error& e){
		std::cout<<std::format("future error:{}",e.what());
	}
}

void use_shared_future(){
	std::promise<int> prom;
	std::shared_future<int> future=prom.get_future();
	std::thread t1(my_func,std::move(prom));//将prom转移给线程t1，线程t1负责设置共享状态的值来通知线程t2和t3
	std::thread t2(thread_func,future);//线程t2和t3共享future对象，线程t2和t3都会等待线程t1设置共享状态的值
	std::thread t3(thread_func,future);
	t1.join();
	t2.join();
	t3.join();
}

void may_throw(){
	throw std::runtime_error("Something went wrong!");
}

void use_future_exception(){
	std::future<void> result(std::async(std::launch::async,may_throw));//使用std::async来构造一个std::future对象，该对象与异步任务相关联
	try{
		result.get();
	}
	catch(const std::exception& e){
		std::cout<<std::format("Exception: {}\n",e.what());
	}
}


int main(){

	//use_async();
	//test_wait_for();
	//use_package();
	//use_promise();
	//use_promise_exception();
	//use_shared_future();
	use_future_exception();


	return 0;
}