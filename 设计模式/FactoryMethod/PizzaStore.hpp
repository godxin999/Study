#pragma once

#include "Pizza.hpp"
#include <memory>


class PizzaStore{
public:
    std::unique_ptr<Pizza> orderPizza(std::string type){
        std::unique_ptr<Pizza> pizza = createPizza(type);
        if(pizza){
            std::cout<<"--- Making a "<<pizza->getName()<<" ---"<<std::endl;
            pizza->prepare();
            pizza->bake();
            pizza->cut();
            pizza->box();
        }
        return pizza;
    }
    virtual std::unique_ptr<Pizza> createPizza(std::string type) = 0;
};