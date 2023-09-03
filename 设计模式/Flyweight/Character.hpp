#pragma once

#include <memory>

class Font;
class Character{
public:
    Character(char c,std::shared_ptr<Font> pFont):c(c),pFont(pFont){}
    char getCharacter()const{
        return c;
    }
private:
    char c;
    std::shared_ptr<Font> pFont;//享元对象的指针
};