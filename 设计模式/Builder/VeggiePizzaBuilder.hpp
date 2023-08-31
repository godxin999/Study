#include "PizzaBuilder.hpp"

class VeggiePizzaBuilder:public PizzaBuilder{
public:
    VeggiePizzaBuilder(){
        pizza->setName("Veggie Pizza");
    }
    virtual void addCheese()override{
        pizza->addTopping("parmesan");
    }
    virtual void addSauce()override{
        pizza->addTopping("sauce");
    }
    virtual void addTomatoes()override{
        pizza->addTopping("chopped tomatoes");
    }
    virtual void addGarlic()override{
        pizza->addTopping("garlic");
    }
    virtual void addPepperoni()override{
        
    }
    virtual void addOlives()override{
        pizza->addTopping("green olives");
    }
    virtual void addSpinach()override{
        pizza->addTopping("spinach");
    }
    virtual void addSausage()override{
        
    }
    virtual ~VeggiePizzaBuilder()=default;
};