#include "PizzaStore.hpp"
#include "NYStylePizza.hpp"

class NYPizzaStore : public PizzaStore{
public:
    std::unique_ptr<Pizza> createPizza(std::string type) override{
        if(type=="NYStylePizza"){
            return std::make_unique<NYStylePizza>(std::make_unique<NYPizzaIngredientFactory>());
        }
        else{
            return nullptr;
        }
    }
};
