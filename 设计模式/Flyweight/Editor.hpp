#pragma once
#include "FontFactory.hpp"
#include "Character.hpp"
#include <list>

class Editor{
public:
    using listIterator=std::list<Character>::iterator;
    void insert(listIterator it,char c,const std::string& fontName,int size,bool bold,bool italic){
        text.insert(it,Character(c,FontFactory::getFont(fontName,size,bold,italic)));
    }
    void erase(listIterator it){
        text.erase(it);
    }
    void erase(size_t index){
        auto it=text.begin();
        std::advance(it,index);
        text.erase(it);
    }
    listIterator begin(){
        return text.begin();
    }
    listIterator end(){
        return text.end();
    }
    std::string getText()const{
        std::string str;
        for(const auto& c:text){
            str.push_back(c.getCharacter());
        }
        return str;
    }
private:
    std::list<Character> text;
};