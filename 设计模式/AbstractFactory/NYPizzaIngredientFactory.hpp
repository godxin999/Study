#include "PizzaIngredientFactory.hpp"

class NYPizzaIngredientFactory : public PizzaIngredientFactory {
public:
    std::unique_ptr<Dough> createDough() override{
        return std::make_unique<ThinCrustDough>();
    }
    std::unique_ptr<Sauce> createSauce() override{
        return std::make_unique<MarinaraSauce>();
    }
    std::unique_ptr<Cheese> createCheese() override{
        return std::make_unique<ReggianoCheese>();
    }
    std::unique_ptr<Clams> createClams() override{
        return std::make_unique<FreshClams>();
    }

};