/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/3 20:07:38
 ************************************************************************/

#include "SingletonTemplate.hpp"
#include <cstring>
#include <chrono>
#include <memory>

class User:public Singleton<User>{
	friend class Singleton<User>;
public:
	virtual ~User()=default;
	void printDetail(){
		std::cout<<std::format("name:{},age:{}\n",name,age);
	}
private:
	User(){}
	User(const std::string& name,int age):name(name),age(age){}
	User(const User&)=delete;
	User& operator=(const User&)=delete;
	std::string name="Unknown User";
	int age=0;
};

void testSingleton(){
	std::thread t1([]() {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			User::getInstance()->printAddress();	
		});

	std::thread t2([]() {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			User::getInstance()->printAddress();
	});

	t1.join();
	t2.join();
}

void test1(){
	std::shared_ptr<User> pUser=User::Construct("godxin999",20);
	pUser->printAddress();
	pUser->printDetail();
	std::shared_ptr<User> pUser2=User::Construct();
	pUser2->printAddress();
	pUser2->printDetail();

	testSingleton();

	std::shared_ptr<User> pUser3=User::getInstance();
	pUser3->printAddress();
	pUser3->printDetail();
}

void test2(){
	std::shared_ptr<User> pUser=User::Construct();
	pUser->printAddress();
	pUser->printDetail();
	std::shared_ptr<User> pUser2=User::Construct("godxin999",20);
	pUser2->printAddress();
	pUser2->printDetail();

	testSingleton();

	std::shared_ptr<User> pUser3=User::getInstance();
	pUser3->printAddress();
	pUser3->printDetail();
}

int main(){
	test1();
	//test2();
	return 0;
}
