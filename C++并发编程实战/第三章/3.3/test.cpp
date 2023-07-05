/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/22 12:35:35
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <thread>
#include <mutex>
#include <memory>
#include <shared_mutex>
#include <map>
using namespace std;
//保护共享数据的初始化
/*
class X{
private:
	connection_info connection_details;
	connection_handle connection;
	std::once_flag connection_init_flag;

	void open_connection(){
		connection=connection_manager.open(connection_details);
	}
public:
	X(const connection_info &details):connection_details(details){}
	void send_data(const data_packet &data){
		std::call_once(connection_init_flag,&X::open_connection,this);//使用call_once和once_flag保证函数只被调用一次
		connection.send_data(data);
	}
	data_packet receive_data(){
		std::call_once(connection_init_flag,&X::open_connection,this);
		return connection.receive_data();
	}
};
*/
//call_once的替代方案
class my_class{};
my_class& get_my_class_instance(){
	static my_class instance;//C++11保证局部静态变量初始化及定义线程安全
	return instance;
}
//使用shared_mutex进行同步
class dns_entry{};
class dns_cache{
	map<string,dns_entry> entries;
	mutable shared_mutex entry_mutex;//支持共享模式和独占模式两种锁定方式
public:
	dns_entry find_entry(const string& domain)const{
		shared_lock<shared_mutex> lk(entry_mutex);//未传递参数，默认以共享模式锁定，多线程可以同时调用find_entry
		auto it=entries.find(domain);
		return (it == entries.end())?dns_entry():it->second;
	}
	void update_or_add_entry(const string& domain,const dns_entry& dns_details){
		lock_guard<shared_mutex> lk(entry_mutex);//以独占模式锁定，在调用update_or_add_entry时，独占锁回阻止其他线程对数据结构修改或调用find_entry
		entries[domain]=dns_details;
	}
};

int main(){






}

