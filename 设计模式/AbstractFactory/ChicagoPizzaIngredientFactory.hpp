#include "PizzaIngredientFactory.hpp"

class ChicagoPizzaIngredientFactory : public PizzaIngredientFactory{
public:
    std::unique_ptr<Dough> createDough() override{
        return std::make_unique<ThickCrustDough>();
    }
    std::unique_ptr<Sauce> createSauce() override{
        return std::make_unique<PlumTomatoSauce>();
    }
    std::unique_ptr<Cheese> createCheese() override{
        return std::make_unique<ReggianoCheese>();
    }
    std::unique_ptr<Clams> createClams() override{
        return std::make_unique<FrozenClams>();
    }
};