#include "Beverage.hpp"

class Decaf : public Beverage{
public:
    Decaf(){
        description="Decaf Coffee";
    }

    double cost()const override{
        return 1.05;
    }

    virtual std::string getDescription()const override{
        return description;
    }
};