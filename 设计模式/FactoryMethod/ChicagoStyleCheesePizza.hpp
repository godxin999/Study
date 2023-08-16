#include "Pizza.hpp"

class ChicagoStyleCheesePizza : public Pizza{
public:
    ChicagoStyleCheesePizza(){
        name="Chicago Style Cheese Pizza";
    }
    void prepare() override{
        std::cout<<"Preparing "<<name<<std::endl;
        dough="Extra Thick Crust Dough";
        sauce="Tomato Sauce";
        toppings.push_back("Cheese");
    }
    void cut() override{
        std::cout<<"Cutting "<<name<<std::endl;
    }
    void bake() override{
        std::cout<<"Baking "<<name<<std::endl;
    }
    void box() override{
        std::cout<<"Boxing "<<name<<std::endl;
    }
};