#include "PizzaStore.hpp"
#include "ChicagoStyleCheesePizza.hpp"

class ChicagoPizzaStore : public PizzaStore {
public:
    std::unique_ptr<Pizza> createPizza(std::string type) override {
        if (type == "cheese") {
            return std::make_unique<ChicagoStyleCheesePizza>();
        } else {
            return nullptr;
        }
    }
};