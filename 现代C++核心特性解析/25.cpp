/*************************************************************************
	> File Name: 25.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/19 17:28:49
 ************************************************************************/

#include <iostream>
#include <string>
#include <thread>
#include <mutex>

std::mutex g_out_lock;

struct RefCount{
    RefCount(const char* f):i(0),func(f){
        std::lock_guard<std::mutex> lk(g_out_lock);
        std::cout<<std::this_thread::get_id()<<"|"<<func<<" : ctor i("<<i<<")"<<"\n";
    }
    ~RefCount(){
        std::lock_guard<std::mutex> lk(g_out_lock);
        std::cout<<std::this_thread::get_id()<<"|"<<func<<" : dtor i("<<i<<")"<<"\n";
    }
    void inc(){
        std::lock_guard<std::mutex> lock(g_out_lock);
        std::cout<<std::this_thread::get_id()<<"|"<<func<<" : inc i("<<i<<")"<<"\n";
        i++;
    }
    int i;
    std::string func;
};
//在同一个线程中，一个线程局部存储对象只会初始化一次，即使其在某个函数中被多次调用。
void foo(const char* f){
    std::string func(f);
    thread_local RefCount tv(func.append("#foo").c_str());
    tv.inc();
}
void bar(const char* f){
    std::string func(f);
    thread_local RefCount tv(func.append("#bar").c_str());
    tv.inc();
}
void thread_func1(){
    const char* func="threadfunc1";
    foo(func);
    foo(func);
    foo(func);
}
void thread_func2(){
    const char* func="threadfunc2";
    foo(func);
    foo(func);
    foo(func);
}
void thread_func3(){
    const char* func="threadfunc3";
    //虽然foo和bar中的线程局部存储对象具有相同的名称，但是由于其不在同一个函数中，因此被认为是相同线程中不同的线程局部存储对象
    foo(func);
    bar(func);
    bar(func);
}

int main(){
    std::thread t1(thread_func1);
    std::thread t2(thread_func2);
    std::thread t3(thread_func3);
    t1.join();
    t2.join();
    t3.join();

    return 0;
}