#include "Monster.hpp"

class DynamicPlayerGeneratedMonster : public Monster{
public:
    DynamicPlayerGeneratedMonster()=default;
    DynamicPlayerGeneratedMonster(const DynamicPlayerGeneratedMonster&)=default;
    virtual void printDetail()override{
        std::cout<<"DynamicPlayerGeneratedMonster"<<std::endl;
    }
    virtual std::unique_ptr<Monster> clone()override{
        return std::make_unique<DynamicPlayerGeneratedMonster>(*this);
    }
    virtual ~DynamicPlayerGeneratedMonster()=default;
};