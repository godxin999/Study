/*************************************************************************
	> File Name: main.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/15 15:25:50
 ************************************************************************/

#include "Sony.hpp"
#include "RCA.hpp"
#include "ConcreteRemote.hpp"


int main(){
	std::unique_ptr<TV> tv=std::make_unique<Sony>();
	std::unique_ptr<RemoteControl> remote=std::make_unique<ConcreteRemote>(std::move(tv));
	remote->on();
	remote->setChannel(5);
	remote->prevChannel();
	remote->nextChannel();
	remote->off();
}