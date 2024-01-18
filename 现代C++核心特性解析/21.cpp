/*************************************************************************
	> File Name: 21.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/18 21:15:01
 ************************************************************************/

#include <iostream>
#include <type_traits>
//C++17之后异常规范作为类型的一部分，实例化模板时也需要注意
void (*fp1)()noexcept=nullptr;
void (*fp2)()=nullptr;
void f1()noexcept{}
void f2(){}
void test1(){
    fp1=f1;
    //fp1=f2;//error，只能绑定noexcept的函数
    fp2=f1;//noexcept的函数可以绑定到普通的函数指针
    fp2=f2;
    //带有noexcept的函数指针和不带有noexcept的函数指针是不同的类型
    std::cout<<std::boolalpha<<std::is_same_v<decltype(fp1), decltype(fp2)><<"\n";//false
}
class A{
public:
    A()=default;
    virtual void f1()noexcept{}
    virtual ~A()=default;
};
class B : public A{
public:
    /*
    void f1()override{
        //warning，使用非noexcept的函数覆盖noexcept的函数
    }
    */
    void f1()noexcept override{}
};
int main(){
    test1();




    return 0;
}