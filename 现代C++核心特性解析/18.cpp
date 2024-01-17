/*************************************************************************
	> File Name: 18.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/16 14:35:11
 ************************************************************************/

#include <iostream>
#include <mutex>
#include <chrono>
#include <condition_variable>
using namespace std::chrono_literals;
bool foo(){
	return true;
}
bool bar(){
	return false;
}
void test1(){
	if(bool b1=foo();b1){
		//...
	}
	else if(bool b2=bar();b2){
		//...
	}
	else{
		//...
	}
	bool flag=true;
	std::mutex mx;
	//通过初始化if对整个if语句块加锁
	if(std::lock_guard<std::mutex> lk(mx);flag){
		flag=false;
	}
}
void test2(){
	std::condition_variable cv;
	std::mutex mx;
	switch(std::unique_lock<std::mutex> lk(mx);cv.wait_for(lk,100ms)){
		case std::cv_status::timeout:
			//...
			break;
		case std::cv_status::no_timeout:
			//...
			break;
	}
}


int main(){





	return 0;
}