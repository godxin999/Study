#pragma once
#include <memory>
#include "ThinCrustDough.hpp"
#include "MarinaraSauce.hpp"
#include "ReggianoCheese.hpp"
#include "FreshClams.hpp"
#include "FrozenClams.hpp"
#include "PlumTomatoSauce.hpp"
#include "ThickCrustDough.hpp"
#include "MozzarellaCheese.hpp"

class Dough;
class Sauce;
class Cheese;
class Clams;
class PizzaIngredientFactory{
public:
    virtual std::unique_ptr<Dough> createDough() = 0;
    virtual std::unique_ptr<Sauce> createSauce() = 0;
    virtual std::unique_ptr<Cheese> createCheese() = 0;
    virtual std::unique_ptr<Clams> createClams() = 0;
    virtual ~PizzaIngredientFactory() = default;
};