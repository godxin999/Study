#pragma once
#include <string>

class Beverage{
public:
    virtual std::string getDescription()const{
        return description;
    }
    virtual double cost()const = 0;
    virtual ~Beverage() = default;
protected:
    std::string description="Unknown Beverage";
};