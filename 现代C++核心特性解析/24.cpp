/*************************************************************************
	> File Name: 24.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/18 22:19:11
 ************************************************************************/

#include <cctype>
#include <compare>
#include <ios>
#include <iostream>
#include <string>
//<=>运算符
void test1(){
    std::cout<<std::boolalpha;
    bool b1 = 7<=>11<0;
    std::cout<<b1<<std::endl;//true
    bool b2 = 17<=>11>0;
    std::cout<<b2<<std::endl;//true
    bool b3 = 17<=>17==0;
    std::cout<<b3<<std::endl;//true
}
//三向比较的返回类型为：std::strong_ordering、std::weak_ordering和std::partial_ordering
//std::strong_ordering表示如果lhs==rhs，那么任何时候lhs和rhs都可以互换，比如基本类型
struct A{
    int a;
    long b;
    //因为类的所有成员的比较结果都是std::strong_ordering，所以结构体A的比较结果也是std::strong_ordering
    auto operator<=>(const A&)const =default;
};
struct B:A{
    short c;
    //因为类的所有成员的比较结果都是std::strong_ordering，所以结构体B的比较结果也是std::strong_ordering
    auto operator<=>(const B&)const =default;
};
//std::weak_ordering表示如果lhs==rhs，那么lhs和rhs不能互换，比如大小不敏感的字符串类
int ci_compare(const char* s1,const char* s2){
    while(std::tolower(*s1)==std::tolower(*s2++)){
        if(*s1++=='\0'){
            return 0;
        }
    }
    return std::tolower(*s1)-std::tolower(*--s2);
}
class CIstring{
public:
    CIstring(const char* str):str_(str){}
    std::weak_ordering operator<=>(const CIstring& rhs)const{
        return ci_compare(str_.c_str(),rhs.str_.c_str())<=>0;
    }
private:
    std::string str_;
};
struct B2:A{
    CIstring si{""};
    //因为CIstring的比较结果是std::weak_ordering，尽管基类成员的比较结果是std::strong_ordering，但是结构体B2的比较结果也是std::weak_ordering
    auto operator<=>(const B2&)const =default;
};
//std::partial_ordering表示如果lhs==rhs，那么lhs和rhs不能互换，其还能给出std::partial_ordering::unordered来表示进行比窘的两个操作数没有关系，比如浮点数
struct B3:A{
    double d;
    CIstring ci{""};
    //因为d的比较结果是std::partial_ordering，尽管基类成员的比较结果是std::strong_ordering，但是结构体B3的比较结果也是std::partial_ordering
    auto operator<=>(const B3&)const =default;
};
void test2(){
    std::cout<<std::boolalpha;
    //NaN和其余浮点数值没有关系
    std::cout<<((0.0/0.0<=>1.0)==std::partial_ordering::unordered);//true
}
//对指针和无作用域枚举类型，其默认的<=>运算符会返回std::strong_ordering
//如果用户定义了三向比较运算符，那么系统会自动生成<、>、<=和>=运算符，==运算符需要用户自己定义，!=运算符会调用根据==运算符自动生成
//对于旧代码，无需重新实现一套<=>和==运算符，三向运算符会调用其<和==运算符进行比较
int main(){
    //test1();
    test2();



    return 0;
}