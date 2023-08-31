#pragma once
#include "Pizza.hpp"
#include <memory>

class PizzaBuilder{
public:
    PizzaBuilder():pizza(std::make_shared<Pizza>()){}
    std::shared_ptr<Pizza> getPizza(){
        return pizza;
    }
    virtual ~PizzaBuilder()=default;
    virtual void addCheese()=0;
    virtual void addSauce()=0;
    virtual void addTomatoes()=0;
    virtual void addGarlic()=0;
    virtual void addPepperoni()=0;
    virtual void addOlives()=0;
    virtual void addSpinach()=0;
    virtual void addSausage()=0;
protected:
    std::shared_ptr<Pizza> pizza;
};
