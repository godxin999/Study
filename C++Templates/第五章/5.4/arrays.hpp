#include <iostream>

int x[]={0,8,15};

template <typename T>
struct MyClass;

template <typename T,std::size_t SZ>
struct MyClass<T[SZ]>{
    static void print(){
        std::cout<<"print() for T["<<SZ<<"]"<<std::endl;
    }
};

template <typename T,std::size_t SZ>
struct MyClass<T(&)[SZ]>{
    static void print(){
        std::cout<<"print() for T(&)["<<SZ<<"]"<<std::endl;
    }
};

template <typename T>
struct MyClass<T[]>{
    static void print(){
        std::cout<<"print() for T[]"<<std::endl;
    }
};

template <typename T>
struct MyClass<T(&)[]>{
    static void print(){
        std::cout<<"print() for T(&)[]"<<std::endl;
    }
};

template <typename T>
struct MyClass<T*>{
    static void print(){
        std::cout<<"print() for T*"<<std::endl;
    }
};
