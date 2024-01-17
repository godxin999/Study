/*************************************************************************
	> File Name: 20.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/17 10:18:48
 ************************************************************************/

#include <cstddef>
#include <iostream>
#include <map>
#include <tuple>

//结构化绑定绑定到原生数组
void test1(){
    int arr[3]{1,2,3};
    auto [x,y,z]=arr;
    std::cout<<x<<y<<z<<std::endl;
}
//结构化绑定绑定到结构体或类对象
//要求非静态数据成员个数与绑定对象个数相同
//要求这些数据成员必须在同一个类或者基类中
//要求绑定的类和结构体中不能存在匿名联合体
class bind_base1{
public:
    int a=42;
    double b=11.7;
};
class bind_test1:public bind_base1{};
class bind_base2{};
class bind_test2:public bind_base2{
public:
    int a=42;
    double b=11.7;
};
class bind_base3{
public:
    int a=42;
};
class bind_test3:public bind_base3{
public:
    double b=11.7;
    //成员函数版本get
    template <std::size_t Idx>
    auto& get()=delete;
    template <>
    auto& get<0>(){
        return a;
    }
    template <>
    auto& get<1>(){
        return b;
    }
};
void test2(){
    auto [x1,y1]=bind_test1{};
    auto [x2,y2]=bind_test2{};
    //auto [x3,y3]=bind_test3{};//error
}
//绑定到元组和类元组的对象
//1．需要满足std::tuple_size<T>::value是一个符合语法的表达式，并且该表达式获得的整数值与标识符列表中的别名个数相同。
//2．类型T还需要保证std::tuple_element<i, T>::type也是一个符合语法的表达式，其中i是小于std::tuple_size<T>::value的整数，表达式代表了类型T中第i个元素的类型。
//3．类型T必须存在合法的成员函数模板get<i>()或者函数模板get<i>(t)，其中i是小于std::tuple_size<T>::value的整数，t是类型T的实例，get<i>()和get<i>(t)返回的是实例t中第i个元素的值
void test3(){
    std::map<int,int> m{{1,2},{3,4}};
    for(const auto& [key,value]:m){
        std::cout<<key<<" "<<value<<std::endl;
    }
}
//实现类元组从而实现结构化绑定
namespace std{
    template<>
    struct tuple_size<bind_test3>{
        static constexpr int value=2;
    };
    template<>
    struct tuple_element<0,bind_test3>{
        using type=int;
    };
    template<>
    struct tuple_element<1,bind_test3>{
        using type=double;
    };
}
/*
//非成员函数版本get
template<std::size_t Idx>
auto& get(bind_test3&)=delete;
template<>
auto& get<0>(bind_test3& t){
    return t.a;
}
template<>
auto& get<1>(bind_test3& t){
    return t.b;
}
*/
void test4(){
    bind_test3 t;
    auto& [x,y]=t;
    x=78;
    std::cout<<t.a<<std::endl;
}
//绑定的访问权限问题
//C++17、C++20中，结构化绑定的访问权限与绑定对象的访问权限相同
struct A{
    friend void test5();
private:
    int a=42;
};
void test5(){
    A a;
    auto& [x]=a;
    std::cout<<x<<std::endl;
}
int main(){
    //test1();
    //test2();
    //test3();
    //test4();
    test5();

    return 0;
}