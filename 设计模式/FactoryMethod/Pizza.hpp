#pragma once

#include <iostream>
#include <cstring>
#include <vector>

class Pizza{
protected:
    std::string name;//名称
    std::string dough;//面团
    std::string sauce;//酱料
    std::vector<std::string> toppings;//佐料
public:
    const std::string& getName()const{
        return name;
    }
    virtual void prepare()=0;
    virtual void bake()=0;
    virtual void cut()=0;
    virtual void box()=0;
    virtual ~Pizza()=default;
};