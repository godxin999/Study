#include "Monster.hpp"
#include <string>
#include <unordered_map>

class MonsterRegistry{
public:
    MonsterRegistry()=default;
    void registerMonster(const std::string &name,std::unique_ptr<Monster> monster){
        Monsters[name]=std::move(monster);
    }
    virtual ~MonsterRegistry()=default;
    std::unordered_map<std::string,std::unique_ptr<Monster>>& getMonsters(){
        return Monsters;
    }
private:
    std::unordered_map<std::string,std::unique_ptr<Monster>> Monsters;
};