#include "TV.hpp"

class Sony:public TV{
public:
    void on() override{
        std::cout << "Sony TV is on" << std::endl;
    }
    void off() override{
        std::cout << "Sony TV is off" << std::endl;
    }
    void tuneChannel(int channel) override{
        std::cout << "Sony TV tuned to channel " << channel << std::endl;
    }
};