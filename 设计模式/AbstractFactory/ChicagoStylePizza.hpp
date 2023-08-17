#include "Pizza.hpp"

class ChicagoStylePizza : public Pizza{
public:
    ChicagoStylePizza(std::unique_ptr<ChicagoPizzaIngredientFactory> ingredientFactory):Pizza("ChicagoStylePizza", std::move(ingredientFactory)){}
    void prepare() override{
        std::cout<<"Preparing"<<std::endl;
        dough = ingredientFactory->createDough();
        sauce = ingredientFactory->createSauce();
        cheese = ingredientFactory->createCheese();
        clams = ingredientFactory->createClams();
    }
    void cut() override{
        std::cout<<"Cutting"<<std::endl;
    }
    void bake() override{
        std::cout<<"Baking"<<std::endl;
    }
    void box() override{
        std::cout<<"Boxing"<<std::endl;
    }
};