#include "Pizza.hpp"

class NYStyleCheesePizza : public Pizza{
public:
    NYStyleCheesePizza(){
        name = "NY Style Cheese Pizza";
    }
    void prepare() override{
        std::cout<<"Preparing "<<name<<std::endl;
        dough = "Thin Crust Dough";
        sauce = "Marinara Sauce";
        toppings.push_back("Grated Reggiano Cheese");
    }
    void bake() override{
        std::cout<<"Baking "<<name<<std::endl;
    }
    void cut() override{
        std::cout<<"Cutting "<<name<<std::endl;
    }
    void box() override{
        std::cout<<"Boxing "<<name<<std::endl;
    }
};