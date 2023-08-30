#pragma once
#include <memory>
#include <iostream>

class Monster{
public:
    Monster()=default;
    virtual std::unique_ptr<Monster> clone()=0;
    virtual void printDetail()=0;
    virtual ~Monster()=default;
};