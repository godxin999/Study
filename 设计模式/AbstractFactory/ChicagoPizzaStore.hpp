#include "PizzaStore.hpp"
#include "ChicagoStylePizza.hpp"

class ChicagoPizzaStore: public PizzaStore {
public:
    std::unique_ptr<Pizza> createPizza(std::string type) override{
        if(type=="ChicagoStylePizza"){
            return std::make_unique<ChicagoStylePizza>(std::make_unique<ChicagoPizzaIngredientFactory>());
        }
        else{
            return nullptr;
        }
    }
};