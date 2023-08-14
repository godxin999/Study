#pragma once
#include "Beverage.hpp"
#include <memory>
class CondimentDecorator:public Beverage{
public:
    CondimentDecorator(std::unique_ptr<Beverage> beverage):beverage(std::move(beverage)){}
    virtual std::string getDescription()const=0;
protected:
    std::unique_ptr<Beverage> beverage;
};