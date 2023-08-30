#include "Monster.hpp"

class WellKnownMonster : public Monster{
public:
    WellKnownMonster()=default;
    WellKnownMonster(const WellKnownMonster&)=default;
    virtual void printDetail()override{
        std::cout<<"WellKnownMonster"<<std::endl;
    }
    virtual std::unique_ptr<Monster> clone()override{
        return std::make_unique<WellKnownMonster>(*this);
    }
    virtual ~WellKnownMonster()=default;
};