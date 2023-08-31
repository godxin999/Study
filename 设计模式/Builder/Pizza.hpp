#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

class Pizza{
public:
    Pizza()=default;
    void addTopping(const std::string& topping){
        toppings.emplace_back(topping);
    }
    void setName(const std::string& name){
        this->name = name;
    }
    void printDetail(){
        std::cout << "Pizza: " << name << std::endl;
        std::cout << "Toppings: " << std::endl;
        for(const auto& topping:toppings){
            std::cout << topping << std::endl;
        }
        std::cout << std::endl;
    }
    void prepare(){
        std::cout << "Preparing " << name << std::endl;
        std::cout << "Adding toppings: " << std::endl;
        for(const auto& topping:toppings){
            std::cout << topping << std::endl;
        }
    }
    void bake(){
        std::cout << "Baking " << name << std::endl;
    }
    void cut(){
        std::cout << "Cutting " << name << std::endl;
    }
    void box(){
        std::cout << "Boxing " << name << std::endl;
    }
    virtual ~Pizza()=default;
protected:
    std::string name;
    std::vector<std::string> toppings;
};