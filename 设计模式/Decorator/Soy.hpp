#include "CondimentDecorator.hpp"

class Soy:public CondimentDecorator{
public:
    Soy(std::unique_ptr<Beverage> beverage):CondimentDecorator(std::move(beverage)){}
    std::string getDescription()const override{
        return beverage->getDescription()+", Soy";
    }
    double cost()const override{
        return beverage->cost()+0.15;
    }
};