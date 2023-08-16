#include "RemoteControl.hpp"
#include <utility>

class ConcreteRemote: public RemoteControl{
protected:
    int currentChannel;
public:
    ConcreteRemote(std::unique_ptr<TV> tv):RemoteControl(std::move(tv)), currentChannel(0){}
    void on() override{
        tv->on();
    }
    void off() override{
        tv->off();
    }
    void setChannel(int channel) override{
        currentChannel = channel;
        tv->tuneChannel(channel);
    }
    void nextChannel() override{
        ++currentChannel;
        tv->tuneChannel(currentChannel);
    }
    void prevChannel() override{
        --currentChannel;
        tv->tuneChannel(currentChannel);
    }
};