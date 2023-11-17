/*************************************************************************
	> File Name: thread_pool.h
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/15 21:20:18
 ************************************************************************/

#pragma once

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <iostream>
#include <algorithm>
#include <thread>
#include <future>
#include <condition_variable>
#include <atomic>
#include <mutex>
#include <queue>
#include <format>
#include <memory>
#include <vector>

class ThreadPool{
public:
	using Task=std::packaged_task<void()>;//任务类型
public:
	//线程池单例
	ThreadPool(const ThreadPool&)=delete;
	ThreadPool& operator=(const ThreadPool&)=delete;
	~ThreadPool(){
		stop();
	}
public:	
	static ThreadPool& instance(){
		static ThreadPool pool;
		return pool;
	}
	//提交任务
	template<typename F,typename... Args>
	auto commit(F&& f,Args&&... args)->std::future<decltype(f(args...))>{//返回一个任务返回类型的std::future
		using ReturnType=decltype(f(args...));
		if(stop_.load()){
			return std::future<ReturnType>();
		}
		auto task=std::make_shared<std::packaged_task<ReturnType()>>(std::bind(std::forward<F>(f),std::forward<Args>(args)...));//将参数绑定，构造一个无参的函数对象
		std::future<ReturnType> future=task->get_future();
		{
			std::lock_guard<std::mutex> cv_lk(cv_mtx_);//对互斥量加锁
			tasks_.emplace([task]{//将任务放入任务队列，task是一个智能指针，捕获后引用计数加一，不会被销毁
				(*task)();//解引用并执行
			});
		}
		cv_lock_.notify_one();//通知一个线程
		return future;
	}
	//空闲线程数
	int idleThreadNum(){
		return thread_num_;
	}
private:
	ThreadPool(unsigned num=12):stop_(false){
		if(num<1)thread_num_=1;
		else thread_num_=num;
		start();
	}
private:
	//向线程池中投放thread_num_个挂起的线程
	void start(){
		for(int i=0;i<thread_num_;++i){
			pool_.emplace_back([this]{
				Task task;
				{
					std::unique_lock<std::mutex> cv_lk(cv_mtx_);//对互斥量加锁
					this->cv_lock_.wait(cv_lk,[this]{//等待条件变量满足
						return this->stop_.load()||!this->tasks_.empty();
					});
					if(this->stop_.load()){//如果线程池结束，直接返回
						return ;
					}
					task=std::move(this->tasks_.front());//从任务队列中取出一个任务
					this->tasks_.pop();//删除该任务
				}
				this->thread_num_--;//空闲线程数减一
				task();
				this->thread_num_++;//空闲线程数加一
			});
		}
	}
	void stop(){
		stop_.store(true);//设置线程池结束标志
		cv_lock_.notify_all();//通知所有线程
		for(auto& t:pool_){
			if(t.joinable()){//等待所有线程结束
				std::cout<<std::format("thread {} join\n",std::hash<std::thread::id>{}(t.get_id()));
				t.join();
			}
		}
	}
private:
	std::mutex cv_mtx_;//互斥量
	std::condition_variable cv_lock_;//条件变量
	std::atomic_bool stop_;//是否结束线程池
	std::atomic_int thread_num_;//线程池中空闲线程数量
	std::queue<Task> tasks_;//任务队列
	std::vector<std::thread> pool_;//线程池
};

#endif