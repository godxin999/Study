/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/16 15:00:55
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <memory>
#include <chrono>

//I.4 当某个对象使用单例时，隐藏的依赖就被注入到对象中，借助依赖注入技术，这个依赖可以变成接口的一部分
//依赖注入的典型方式为构造函数、设置函数(setter)或模板参数
//下例通过依赖注入替换一个日志记录器

class Logger{
public:
	virtual void printToLog(const std::string&)=0;
	virtual ~Logger()=default;
};

class SimpleLoger:public Logger{
	void printToLog(const std::string& msg) override{
		std::cout<<msg<<std::endl;
	}
};

class TimeLogger:public Logger{
	using mySecondTick=std::chrono::duration<long double>;
	long double timeSinceEpoch(){
		auto timeNow=std::chrono::system_clock::now();
		auto duration=timeNow.time_since_epoch();
		mySecondTick sec(duration);
		return sec.count();
	}

	void printToLog(const std::string& msg) override{
		std::cout<<std::fixed;
		std::cout<<"Time since epoch: "<<timeSinceEpoch()<<" : "<<msg<<std::endl;
	}
};

class Client{
public:
	Client(std::shared_ptr<Logger> log):logger(log){}
	void doSomething(){
		logger->printToLog("This is message.");
	}
	void setLogger(std::shared_ptr<Logger> log){//客户端代码支持通过构造函数和setter来注入日志记录服务
		logger=log;
	}
private:
	std::shared_ptr<Logger> logger;
};

int main(){

	std::cout<<std::endl;

	Client c(std::make_shared<SimpleLoger>());
	c.doSomething();
	c.setLogger(std::make_shared<TimeLogger>());
	c.doSomething();
	c.doSomething();

	std::cout<<std::endl;



}