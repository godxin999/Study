#include "TV.hpp"

class RCA:public TV{
public:
    void on() override{
        std::cout << "RCA TV is on" << std::endl;
    }
    void off() override{
        std::cout << "RCA TV is off" << std::endl;
    }
    void tuneChannel(int channel) override{
        std::cout << "RCA TV tuned to channel " << channel << std::endl;
    }

};