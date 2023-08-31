#include "PizzaBuilder.hpp"

class MeatPizzaBuilder:public PizzaBuilder{
public:
    MeatPizzaBuilder(){
        pizza->setName("Meat Pizza");
    }
    virtual void addCheese()override{
        pizza->addTopping("mozzerella");
    }
    virtual void addSauce()override{
        pizza->addTopping("NY style sauce");
    }
    virtual void addTomatoes()override{
        pizza->addTopping("sliced tomatoes");
    }
    virtual void addGarlic()override{
        pizza->addTopping("garlic");
    }
    virtual void addPepperoni()override{
        pizza->addTopping("pepperoni");
    }
    virtual void addOlives()override{

    }
    virtual void addSpinach()override{

    }
    virtual void addSausage()override{
        pizza->addTopping("sausage");
    }
    virtual ~MeatPizzaBuilder()=default;
};