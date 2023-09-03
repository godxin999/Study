#pragma once

#include <string>

//享元对象类
//因为不能对每个字符都单独存储一个字体对象，这会导致大量的内存占用
class Font{
public:
    Font(const std::string& fontName,int size,bool Bold,bool Italic):fontName(fontName),size(size),Bold(Bold),Italic(Italic){}
    std::string getFontName() const{
        return fontName;
    }
    int getSize() const{
        return size;
    }
    bool isBold() const{
        return Bold;
    }
    bool isItalic() const{
        return Italic;
    }
private:
    std::string fontName;
    int size;
    bool Bold;
    bool Italic;
};