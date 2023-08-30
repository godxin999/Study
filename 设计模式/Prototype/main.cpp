/*************************************************************************
	> File Name: main.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/30 20:46:39
 ************************************************************************/

#include "WellKnownMonster.hpp"
#include "DynamicPlayerGeneratedMonster.hpp"
#include "MonsterMaker.hpp"


int main(){
	MonsterRegistry registry;

	registry.registerMonster("WellKnownMonster",std::make_unique<WellKnownMonster>());
	registry.registerMonster("DynamicPlayerGeneratedMonster",std::make_unique<DynamicPlayerGeneratedMonster>());

	MonsterMaker maker(registry);

	std::unique_ptr<Monster> monster=maker.createMonster();
	if(monster){
		monster->printDetail();
	}
	return 0;
}