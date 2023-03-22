#pragma once

#include "ZooAniaml.h"

class Raccoon :public virtual ZooAnimal {//虚继承
public:
	Raccoon() :pettable_flag(false) {//默认构造函数
		std::cout << "Raccoon()" << std::endl;
	}
	Raccoon(std::string name, bool onExhibit) :ZooAnimal(name, "Raccoon", onExhibit), pettable_flag(false) {//构造虚基类部分
		std::cout << "Raccoon(std::string name, bool onExhibit)" << std::endl;
	}
	virtual std::ostream& print(std::ostream&)const {
		return std::cout << "Raccoon::print" << std::endl;
	}
	bool pettable()const {
		return pettable_flag;
	}
	void pettable(bool petval) {
		pettable_flag = petval;
	}
private:
	bool pettable_flag;
};