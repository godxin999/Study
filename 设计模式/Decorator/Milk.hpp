#include "CondimentDecorator.hpp"

class Milk:public CondimentDecorator{
public:
    Milk(std::unique_ptr<Beverage> beverage):CondimentDecorator(std::move(beverage)){}
    std::string getDescription()const override{
        return beverage->getDescription()+", Milk";
    }
    double cost()const override{
        return beverage->cost()+0.10;
    }

};