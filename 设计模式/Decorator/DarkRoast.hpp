#include "Beverage.hpp"

class DarkRoast : public Beverage{
public:
    DarkRoast(){
        description="Dark Roast Coffee";
    }

    double cost()const override{
        return 0.99;
    }

    virtual std::string getDescription()const override{
        return description;
    }
};