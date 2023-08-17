#include "Pizza.hpp"

class NYStylePizza : public Pizza{
public:
    NYStylePizza(std::unique_ptr<PizzaIngredientFactory> ingredientFactory):Pizza("NYStylePizza",std::move(ingredientFactory)){}
    void prepare() override{
        std::cout << "Preparing " << name << std::endl;
        dough = ingredientFactory->createDough();
        sauce = ingredientFactory->createSauce();
        cheese = ingredientFactory->createCheese();
    }
    void bake() override{
        std::cout << "Baking" << std::endl;
    }
    void cut() override{
        std::cout << "Cutting" << std::endl;
    }
    void box() override{
        std::cout << "Boxing" << std::endl;
    }
    
};