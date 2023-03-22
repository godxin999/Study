#pragma once
#include "Bear.h"
#include "Raccoon.h"
#include "Endangered.h"
/*
class Panda :public Bear, public Endangered {
public:
	Panda() = default;
	Panda(std::string name, bool onExhibit = true) :Bear(name, "Panda", onExhibit) {}
	virtual ~Panda() {
		std::cout << "Panda dtor" << std::endl;
	}
	virtual std::ostream& print()const {
		return std::cout << "Panda::print" << std::endl;
	}
	virtual void highlight()const {
		std::cout << "Panda::highlight" << std::endl;
	}
	virtual double max_weight()const {
		return std::max(ZooAnimal::max_weight(), Endangered::max_weight());
	}
	virtual int toes() {
		std::cout << "Panda::toes" << std::endl;
		return 0;
	}
	virtual void cuddle() {
		std::cout << "Panda::cuddle" << std::endl;
	}

};
*/
class Panda :public Bear,
			 public Raccoon, public Endangered {
public:
	Panda() :sleeping_flag(false) {
		std::cout << "Panda()" << std::endl;
	}//默认构造函数
	Panda(std::string name, bool onExhibit = true)/*构造虚基类部分*/
		:ZooAnimal(name, "Panda", onExhibit),
		 Bear(name, onExhibit),
		 Raccoon(name, onExhibit),
		 Endangered(Endangered::critical),
		 sleeping_flag(false) {
		std::cout << "Panda(std::string name, bool onExhibit)" << std::endl;
	}
	virtual ~Panda() {
		std::cout << "Panda dtor" << std::endl;
	}
	virtual std::ostream& print()const {
		return std::cout << "Panda::print" << std::endl;
	}
	virtual void highlight()const {
		std::cout << "Panda::highlight" << std::endl;
	}
	virtual double max_weight()const {
		return std::max(ZooAnimal::max_weight(), Endangered::max_weight());
	}
	virtual int toes() {
		std::cout << "Panda::toes" << std::endl;
		return 0;
	}
	virtual void cuddle() {
		std::cout << "Panda::cuddle" << std::endl;
	}
	bool sleeping() const { return sleeping_flag; }
	void sleeping(bool newval) { sleeping_flag = newval; }
private:
	bool sleeping_flag;
};