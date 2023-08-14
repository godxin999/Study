#include "Beverage.hpp"

class Espresso:public Beverage{
public:
    Espresso(){
        description="Espresso";
    }

    double cost()const override{
        return 1.99;
    }

    virtual std::string getDescription()const override{
        return description;
    }
};