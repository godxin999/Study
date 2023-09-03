#include "Font.hpp"
#include <memory>
#include <unordered_set>


//通过Factory来获取享元对象
class FontFactory{
public:
    static std::shared_ptr<Font> getFont(const std::string& fontName,int size,bool bold,bool italic){
        auto font=std::make_shared<Font>(fontName,size,bold,italic);
        auto it=fontSet.find(font);
        if(it!=fontSet.end()){//如果已经存在，就返回已经存在的对象
            return *it;
        }
        else{
            fontSet.insert(font);//如果不存在，就插入到集合中
            return font;
        }
    }
private:
    static std::unordered_set<std::shared_ptr<Font>> fontSet;
};
