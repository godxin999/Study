#include "Beverage.hpp"

class HouseBlend : public Beverage{

public:
    HouseBlend(){
        description="House Blend Coffee";
    }

    double cost()const override{
        return 0.89;
    }

    virtual std::string getDescription()const override{
        return description;
    }
};