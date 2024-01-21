/*************************************************************************
	> File Name: 31.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/21 22:35:23
 ************************************************************************/

#include <iostream>
//C++标准属性
//[[attr]] [[attr1,attr2,attr3(args)]] [[namespace::attr]]
//[[noreturn]]属性表示函数不会返回，即执行流会在这个函数中断，函数不会返回到其调用者
//[[carries_dependency]]可以作为函数或者lambda表达式参数的属性，表示调用者不用担心内存顺序，函数内部会处理好这个问题，编译器可以不生成栅栏指令，或者作为函数的属性，表示函数的返回值已经处理好内存顺序，不需要编译器在函数返回前插入内存栅栏指令
//[[deprecated("func was deprecated ,use other func instead")]]属性表示函数或者类型已经被弃用，编译器会在使用这个函数或者类型时发出警告
//[[fallthrough]]属性表示有意在switch语句中进行直落
//[[nodiscard]]属性表示函数的返回值不应该被忽略，C++20支持字符串参数来提示不要弃值的原因
//C++20中nodiscard可以用于构造函数，其会在构造临时对象时发出警告
//[[maybe_unused]]属性表示变量或者函数可能不会被使用
int foo(int a[[maybe_unused]],int b[[maybe_unused]]){
    return 5;
}
void test1(){
    foo(1,2);
}
//[[likely]]和[[unlikely]]属性表示条件表达式的可能性，编译器可以根据这个属性来优化代码
constexpr int factorial(int n){
    if(n>1)[[likely]]{
        return n*factorial(n-1);
    }
    else[[unlikely]]{
        return 1;
    }
}
//[[no_unique_address]]属性表示类的成员变量不需要独立的地址，可以和其他成员变量共用地址，该属性声明的对象必须是非静态数据成员，且不为位域
struct empty1{};
struct empty2{};
struct A{
    int a;
    [[no_unique_address]] empty1 e1;
    [[no_unique_address]] empty2 e2;//C++20下该属性被msvc忽略
};
void test2(){
    std::cout<<sizeof(A)<<std::endl;//g++编译输出4
}

//对于无状态的类，这种类不需要有数据成员，唯一需要做的就是实现一些必要的函数，例如仿函数。当这种类作为数据成员加入其他类时，会占据独一无二的内存地址。所以，在C++20的环境下，我们可以使用no_unique_address属性，让其不需要占用额外的内存地址空间。
struct Functor{
    int operator()(){
        return 42;
    }
};
struct Container{
    int data;
    [[no_unique_address]] Functor f;
};
void test3(){
    Container c{10};
    std::cout<<sizeof(c)<<std::endl;//g++，4
    std::cout<<c.f()<<std::endl;
}
int main(){
    //test1();
    //test2();
    test3();



    return 0;
}