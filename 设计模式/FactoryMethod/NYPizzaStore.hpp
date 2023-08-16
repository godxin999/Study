#include "PizzaStore.hpp"
#include "NYStyleCheesePizza.hpp"
class NYPizzaStore : public PizzaStore{
public:
    std::unique_ptr<Pizza> createPizza(std::string type) override{
        if(type == "cheese"){
            return std::make_unique<NYStyleCheesePizza>();
        }
        else{
            return nullptr;
        }
    }
};