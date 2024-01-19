/*************************************************************************
	> File Name: 27.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/19 21:31:14
 ************************************************************************/

#include <iostream>
#include <type_traits>
//constexpr构造函数
//1.构造函数需用constexpr声明
//2.构造函数体必须为空
//3.构造函数初始化列表中必须是常量表达式
//constexpr声明的自定义类型的变量需要满足析构函数是平凡的
//1.不能有用户自定义的析构函数
//2.析构函数不能是虚函数
//3.基类和成员的析构函数必须是平凡的
class X{
public:
    constexpr X():x(100){}
    explicit constexpr X(int i):x(i){}
    [[nodiscard]]constexpr int get()const{
        return x;
    }
private:
    int x;
};
void test1(){
    constexpr X x;
    int arr[x.get()];
}
//constexpr lambdas
//C++17后lambda表达式在条件允许的情况下都会隐式声明为constexpr
//可以强制指定constexpr，从而检查lambda表达式是否是常量表达式
auto get_size=[](int i)constexpr{
    return i*2;
};
void test2(){
    constexpr int size=get_size(10);
    std::cout<<size<<"\n";
}
//constexpr的内联属性
class K{
public:
    static constexpr int num{5};
    //inline static constexpr int num{5};//等价于这一行
};
//if constexpr
template <typename T>
auto minus(T a,T b){
    if constexpr(std::is_same_v<T, double>){
        if(std::fabs(a-b)<1e-4){
            return 0.;
        }
        else{
            return a-b;
        }
    }
    else{
        //这个return必须要在else中，否则可能编译失败(编译出两条return语句)
        return static_cast<int>(a-b);
    }
}
//if constexpr不支持短路规则
template <typename T>
auto any2i(T t){
    if constexpr(std::is_same_v<T, std::string>){
        //嵌套if constexpr来替换短路运算
        if constexpr(T::npos==-1){
            return atoi(t.c_str());
        }
    }
    else{
        return t;
    }
}
//C++20允许constexpr虚函数
struct B{
    [[nodiscard]]virtual constexpr int f()const{
        return 1;
    }
};
struct D:B{
    [[nodiscard]]constexpr int f()const override {
        return 2;
    }
};
constexpr D d;
constexpr B const* b=&d;
void test3(){
    int arr[b->f()];
    int arr2[d.f()];
}
//C++20之后允许在constexpr中进行平凡的默认初始化
struct T{
    int x;
};
constexpr void test4(){
    //C++17报错，t没有初始化，需要提供一个构造函数
    T t;
}
//C++20之后允许在constexpr中更改联合类型的有效成员
union U{
    int i;
    float f;
};
constexpr void test5(){
    U u{};
    u.i=3;
    u.f=1.2f;
}
//使用consteval声明立即函数
consteval int sqr(int n){
    return n*n;
}
consteval int sqrsqr(int n){
    return sqr(sqr(n));
}
void test6(){
    constexpr int n=sqr(10);
    int x=100;
    //constexpr int m=sqr(x);//编译失败，x不是常量表达式
    constexpr int m=sqrsqr(10);
    auto f=[](int n)consteval{
        return n*n;
    };
    constexpr int o=f(100);
}
//constinit用于静态存储时间的变量声明，其要求变量具有常量初始化程序
constinit int m=100;//constinit初始化的对象不要求具有常量属性
const char* f(){
    return "hello";
}
constexpr const char* g(){
    return "hello";
}
//constinit const char* str1=f();//编译失败，f()不是一个常量初始化程序
constinit const char* str2=g();
void test7(){
    constinit static int y=42;
    //constinit int z=1;//error，局部变量不是静态的
}
//constinit还可以用于非初始化声明，以告知编译器thread_local变量已被初始化
extern thread_local constinit int x;
//C++20中的std::is_constant_evaluated()函数
//用于判断当前表达式是否处于明显常量求值环境中
constexpr double power(double b,int x){
    if(std::is_constant_evaluated()&&x>=0){
        double r=1.,p=b;
        unsigned u=static_cast<unsigned>(x);
        while(u!=0){
            if(u&1)r*=p;
            u/=2;
            p*=p;
        }
        return r;
    }
    else{
        return std::pow(b,static_cast<double>(x));
    }
}
void test8(){
    constexpr double a=power(10., 3);
    int n=3;
    double b=power(10.,n);
}
//如果判断是否为明显常量求值时存在多个条件，std::is_constant_evaluated()会试探两种情况求值
void test9(){
    int y;
    //当返回true时，y会改变a的值，那么其返回false，在运行时初始化
    const int a=std::is_constant_evaluated()?y:1;//1
    //当返回true时，b的值恒定为2，所以在编译时初始化
    const int b=std::is_constant_evaluated()?2:y;//2
    std::cout<<a<<" "<<b<<"\n";
}
int main(){
    //test1();
    //test2();
    //test3();
    //test4();
    //test5();
    //test6();
    //test8();
    test9();


    return 0;
}