#pragma once

#include <memory>
#include "TV.hpp"

class RemoteControl{
protected:
    std::unique_ptr<TV> tv;
public:
    RemoteControl(std::unique_ptr<TV> tv):tv(std::move(tv)){}
    virtual void on() = 0;
    virtual void off() = 0;
    virtual void setChannel(int channel) = 0;
    virtual void prevChannel()=0;
    virtual void nextChannel()=0;
    virtual ~RemoteControl()=default;
};