#include <string>


class BoolString{
private:
    std::string value;
public:
    BoolString(const std::string& s):value(s){}
    template <typename T=std::string>
    T get()const{
        return value;
    }
};

//成员模板的特例化
//我们只能对特例化的版本进行定义，特例化的成员模板就是一个函数，如果被多个文件包含则会发生重复定义，为了避免这种情况，我们把其定义为inline的
template <>
inline bool BoolString::get<bool>() const{
    return value=="true"||value=="1"||value=="on";
}