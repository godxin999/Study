#include "CondimentDecorator.hpp"

class Whip:public CondimentDecorator{
public:
    Whip(std::unique_ptr<Beverage> beverage):CondimentDecorator(std::move(beverage)){}
    std::string getDescription()const override{
        return beverage->getDescription()+", Whip";
    }
    double cost()const override{
        return beverage->cost()+0.10;
    }
};