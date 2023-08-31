#include "PizzaBuilder.hpp"

class PizzaDirector{
public:
    PizzaDirector(std::unique_ptr<PizzaBuilder> pizzaBuilder):pizzaBuilder(std::move(pizzaBuilder)){}
    std::shared_ptr<Pizza> makePizza(){
        pizzaBuilder->addSauce();
        pizzaBuilder->addCheese();
        pizzaBuilder->addTomatoes();
        pizzaBuilder->addGarlic();
        pizzaBuilder->addOlives();
        pizzaBuilder->addSpinach();
        pizzaBuilder->addPepperoni();
        pizzaBuilder->addSausage();
        return pizzaBuilder->getPizza();
    }
    void setPizzaBuilder(std::unique_ptr<PizzaBuilder> pizzaBuilder){
        pizzaBuilder=std::move(pizzaBuilder);
    }
private:
    std::unique_ptr<PizzaBuilder> pizzaBuilder;
};