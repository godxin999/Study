#pragma once
#include "ZooAniaml.h"
/*
using Dancetype = unsigned;
const Dancetype two_left_feet = 0;
const Dancetype Astaire = 1;
const Dancetype Rogers = 42;


class Bear :public ZooAnimal {
public:
	Bear() :dancetype(Rogers) {}
	Bear(std::string name, std::string family = "Bear", bool onExhibit = true) :ZooAnimal(name, family, onExhibit), dancetype(two_left_feet) {}
	virtual std::ostream& print()const {
		return std::cout << "Bear::print" << std::endl;
	}
	virtual int toes()const {
		std::cout << "Bear::toes" << std::endl;
		return 0;
	}
	int mumble(int) {
		std::cout << "Bear::mumble" << std::endl;
		return 0;
	}
	void dance(Dancetype)const {
		std::cout << "Bear::dance" << std::endl;
	}
	virtual ~Bear() {
		std::cout << "Bear dtor" << std::endl;
	}

private:
	Dancetype dancetype;
};
*/

class Bear :virtual public ZooAnimal {//虚继承
public:
	enum Dancetype { two_left_feet, macarena, fandango };
	Bear() :dancetype(two_left_feet) {//一定要定义默认构造函数
		std::cout << "Bear()" << std::endl;
	}
	Bear(std::string name, bool onExhibit = true) :ZooAnimal(name, "Bear", onExhibit), dancetype(two_left_feet) {//直接初始化虚基类部分
		std::cout << "Bear(std::string name, bool onExhibit = true)" << std::endl;
	}
	virtual std::ostream& print()const {
		return std::cout << "Bear::print" << std::endl;
	}
	virtual int toes()const {
		std::cout << "Bear::toes" << std::endl;
		return 0;
	}
	int mumble(int) {
		std::cout << "Bear::mumble" << std::endl;
		return 0;
	}
	void dance(Dancetype)const {
		std::cout << "Bear::dance" << std::endl;
	}
	virtual ~Bear() {
		std::cout << "Bear dtor" << std::endl;
	}

private:
	std::string name;
	Dancetype dancetype;
};