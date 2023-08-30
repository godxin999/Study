#include "MonsterRegistry.hpp"
#include <random>

class MonsterMaker{
public:
    MonsterMaker(MonsterRegistry& registry):registry(registry){}
    virtual ~MonsterMaker()=default;
    std::unique_ptr<Monster> createMonster(){
        if(registry.getMonsters().empty()){
            return nullptr;
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0,registry.getMonsters().size()-1);
        auto it=registry.getMonsters().begin();
        std::advance(it,dis(gen));//前进随机步长
        return it->second->clone();
    }
private:
    MonsterRegistry& registry;
};