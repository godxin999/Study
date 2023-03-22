#pragma once
#include <iostream>
#include <algorithm>
#include <cstring>

class ZooAnimal {
public:
	ZooAnimal() :exbibit_stat(false) {
		std::cout << "ZooAnimal()" << std::endl;
	}
	ZooAnimal(std::string animal, std::string family, bool exhibit) :nm(animal), fam_name(family), exbibit_stat(exhibit) {
		std::cout << "ZooAnimal(std::string animal, std::string family, bool exhibit)" << std::endl;
	}
	virtual ~ZooAnimal() {
		std::cout << "Animal dtor" << std::endl;
	}
	virtual std::ostream& print()const {
		return std::cout << "Animal::print" << std::endl;
	}
	virtual int population()const {
		std::cout << "Animal::population" << std::endl;
		return 0;
	}
	virtual double max_weight()const {
		std::cout << "Animal::max_weight" << std::endl;
		return 0;
	}
	std::string name()const { return nm; }
	std::string family_name()const { return fam_name; }
	bool onExhibit()const { return exbibit_stat; }

protected:
	std::string nm;
	std::string fam_name;
	bool exbibit_stat;
};