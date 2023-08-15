#pragma once
#include <iostream>

class TV{
public:
    virtual void on(){}
    virtual void off(){}
    virtual void tuneChannel(int channel){}
    virtual ~TV()=default;
};