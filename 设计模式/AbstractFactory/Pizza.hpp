#pragma once
#include <iostream>
#include <cstring>
#include <memory>
#include "NYPizzaIngredientFactory.hpp"
#include "ChicagoPizzaIngredientFactory.hpp"
class Dough;
class Sauce;
class Cheese;
class Clams;

class Pizza{
protected:
    std::string name;//名称
    std::unique_ptr<Dough> dough;//面团
    std::unique_ptr<Sauce> sauce;//酱料
    std::unique_ptr<Cheese> cheese;//奶酪
    std::unique_ptr<Clams> clams;
protected:
    std::unique_ptr<PizzaIngredientFactory> ingredientFactory;
public:
    Pizza(const std::string& name,std::unique_ptr<PizzaIngredientFactory> ingredientFactory):name(name),ingredientFactory(std::move(ingredientFactory)){}
    const std::string& getName()const{
        return name;
    }
    virtual void prepare()=0;
    virtual void bake()=0;
    virtual void cut()=0;
    virtual void box()=0;
    virtual ~Pizza()=default;
};