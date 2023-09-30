/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/30 11:39:32
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//R.1 使用资源句柄和RAII自动管理资源
class ResourceGuard{
public:
    explicit ResourceGuard(const std::string& r):resource(r){
        std::cout<<"Acquire the "<<resource<<"."<<std::endl;
    }
    ~ResourceGuard(){
        std::cout<<"Release the "<<resource<<"."<<std::endl;
    }

private:
    std::string resource;
};

void test1(){
    std::cout<<std::endl;
    ResourceGuard guard1("memblock1");
    std::cout<<"before local scope"<<std::endl;
    {
        ResourceGuard guard2("memblock2");
    }
    std::cout<<"after local scope"<<std::endl;

    std::cout<<std::endl;

    std::cout<<"before try-catch block"<<std::endl;
    try{
        ResourceGuard guard3("memblock3");
        throw std::exception();
    }catch(const std::exception& e){
        std::cout<<"catch exception"<<std::endl;
    }
    std::cout<<"after try-catch block"<<std::endl;
}

//R.3/R.4 原始指针(T*)和原始引用(T&)不表示所有权
//R.5 优先使用有作用域的对象，不做非必要的堆上分配
class Gadget{
public:
    explicit Gadget(int n):n(n){}
private:
    int n;
};
void f(int n){
    auto *p=new Gadget{n};
    //费时、易错
    delete p;
}
void f2(int n){
    Gadget g{n};
    //...
}

int main(){
    test1();

    return 0;
}