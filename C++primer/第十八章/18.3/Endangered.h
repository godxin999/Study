#pragma once
/*
class Endangered {
public:
	virtual ~Endangered() {
		std::cout << "Endangered dtor" << std::endl;
	}
	virtual std::ostream& print()const {
		return std::cout << "Endangered::print" << std::endl;
	}
	virtual void highlight()const {
		std::cout << "Endangered::highlight" << std::endl;
	}
	virtual double max_weight()const {
		std::cout << "Endangered::max_weight" << std::endl;
		return 0;
	}
};
*/
class Endangered {
public:
	enum Status { critical, environment, improving };
	Endangered(Status stat = improving) :animal_status(stat) {
		std::cout << "Endangered(Status stat)" << std::endl;
	}
	virtual ~Endangered() {
		std::cout << "Endangered dtor" << std::endl;
	}
	virtual std::ostream& print()const {
		return std::cout << "Endangered::print" << std::endl;
	}
	virtual void highlight()const {
		std::cout << "Endangered::highlight" << std::endl;
	}
	virtual double max_weight()const {
		std::cout << "Endangered::max_weight" << std::endl;
		return 0;
	}
private:
	Status animal_status;
};