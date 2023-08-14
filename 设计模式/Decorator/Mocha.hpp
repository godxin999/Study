#include "CondimentDecorator.hpp"

class Mocha:public CondimentDecorator{
public:
    Mocha(std::unique_ptr<Beverage> beverage):CondimentDecorator(std::move(beverage)){}
    std::string getDescription()const override{
        return beverage->getDescription()+", Mocha";
    }
    double cost()const override{
        return beverage->cost()+0.20;
    }

};