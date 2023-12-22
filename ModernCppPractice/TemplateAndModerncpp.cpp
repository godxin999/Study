#include <map>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <type_traits>

//C++11支持变长参数模板
template <typename... Args>
void log_message(const Args&... args){
    std::ostringstream stream;
    (void)std::initializer_list<int>{(void(stream <<"["<<args<<"]"), 0)...};
    stream<<std::endl;
    std::cout<<stream.str();
}
void test1(){
    log_message("1",2,3,"4");
    log_message();
}

template <typename T>
bool is_nullptr(T){
    return false;
}
template <>
bool is_nullptr(std::nullptr_t){
    return true;
}
void test2(){
    std::cout<<std::boolalpha;
    std::cout<<is_nullptr(NULL)<<std::endl;
    std::cout<<is_nullptr(nullptr)<<std::endl;
}

template <typename Cont,typename Func>
void for_each(Cont&& cont,Func func){
    for(const auto& elem:cont){
        func(elem);
    }
}
void test3(){
    std::vector<int> vec{1,2,3,4,5};
    auto print=[](const auto& value){
        std::cout<<value<<" ";
    };
    for_each(vec,print);
}

#include <limits>
//变量模板
template <typename T>
constexpr T min_positive_value=std::numeric_limits<T>::min();
void test4(){
    std::cout<<min_positive_value<int><<std::endl;
    std::cout<<min_positive_value<float><<std::endl;
    std::cout<<min_positive_value<double><<std::endl;
    std::cout<<min_positive_value<long double><<std::endl;
}

//C++17支持auto作为函数返回值，而不仅限于模板
auto getFirst(const std::vector<int>& vec){
    return vec.empty()?0:vec.front();
}
void test5(){
    std::vector<int> vec{1,2,3,4,5};
    std::cout<<getFirst(vec)<<std::endl;
    std::cout<<getFirst(std::vector<int>{})<<std::endl;
}

#include <concepts>
//auto配合C++20的概念约束
template <std::integral T>
auto square(T value){
    return value*value;
}
void test6(){
    std::cout<<square(10)<<std::endl;
    //std::cout<<square(10.0)<<std::endl;//不满足std::integral概念
}

template <typename T1,typename T2>
void print_pair(const std::pair<T1,T2>& p){
    auto [first,second]=p;
    std::cout<<first<<" "<<second<<std::endl;
}
void test7(){
    std::pair<int,std::string> p{1,"hello"};
    print_pair(p);
}

//constexpr if 终止递归条件
template <int N>
constexpr int factorial(){
    if constexpr(N>1){
        return N* factorial<N-1>();
    }
    else{
        return 1;
    }
}
void test8(){
    std::cout<<factorial<5>()<<std::endl;
}

//折叠表达式
template <typename... Args>
void log_message2(const Args&... args){
    std::ostringstream stream;
    ((stream<<"["<<args<<"]"),...);
    stream<<std::endl;
    std::cout<<stream.str();
}
void test9(){
    log_message2("1",2,3,"4");
    log_message2();
}

//一元右折叠
//(pack op ...) op是一个二元操作符，它的展开方式如下：
//(e1 op (e2 op (... op (eN-1 op eN))))
template <typename... Args>
auto all_true(const Args&... args){
    return (args&&...);
}
//一元左折叠
//(... op pack) op是一个二元操作符，它的展开方式如下：
//((((e1 op e2) op e3) op ...) op eN)
template <typename... Args>
auto sum(Args... args){
    return (...+args);
}
//二元折叠
//(init op ... op pack) op是一个二元操作符，它的展开方式如下：
//((((init op e1) op e2) op ...) op eN)
//(pack op ... op init) op是一个二元操作符，它的展开方式如下：
//(e1 op (e2 op (... op (eN op init))))
template <typename... Args>
auto subtract_from(int init,Args... args){
    return (init-...-args);
}

template <typename... Args>
std::string concatenate(const Args&... args){
    return (std::string(args)+...);
}
void test10(){
    std::cout<<std::boolalpha;
    std::cout<<all_true(true,true,true)<<std::endl;
    std::cout<<all_true(true,true,false)<<std::endl;
    std::cout<<sum(1,2,3,4,5)<<std::endl;
    std::cout<<subtract_from(100,1,2,3,4,5)<<std::endl;
    std::cout<<concatenate("hello"," ","world")<<std::endl;
}

//C++20的概念约束
template <typename T>
concept Sortable=requires(T a,T b){
    {a<b}->std::convertible_to<bool>;
};
template <Sortable T>
void sort_and_print(std::vector<T>& vec){
    std::sort(vec.begin(),vec.end());
    for(const auto& elem:vec){
        std::cout<<elem<<" ";
    }
    std::cout<<std::endl;
}
void test11(){
    std::vector<int> vec{1,3,2,5,4};
    sort_and_print(vec);
}

template <typename T>
concept Addable=requires(T a,T b){
    {a+b}->std::convertible_to<T>;
};
template <Addable T>
T add(T a,T b){
    return a+b;
}
void test12(){
    std::cout<<add(10,20)<<std::endl;
    std::cout<<add(10.0,20.0)<<std::endl;
}

template <typename T>
class MyValue{
public:
    T value;
    MyValue(T value):value(value){}

    auto operator<=>(const MyValue& other)const = default;//因为定义了<=>，所以编译器也会自动生成==
    //如果需要，我们可以显式地定义==
    //bool operator==(const MyValue& other)const = default;
};
void test13(){
    MyValue<int> v1(10);
    MyValue<int> v2(20);
    std::cout<<std::boolalpha;
    std::cout<<(v1<v2)<<std::endl;
    std::cout<<(v1==v2)<<std::endl;
}

//模板lambda
auto generic_lambda=[]<typename T>(T value){
    std::cout<<value<<std::endl;
};
void test14(){
    generic_lambda(10);
    generic_lambda(10.0);
    generic_lambda("hello");
}
//lambda使用概念和requires
auto generic_lambda_int=[]<typename T>(T value)requires std::integral<T>{
    std::cout<<value<<std::endl;
};
void test15(){
    generic_lambda_int(10);
    //generic_lambda_int(10.0);//不满足std::integral概念
}
//可复制的lambda
auto copy_lambda=[](){
    std::cout<<"copy"<<std::endl;
};
void test16(){
    auto copy_lambda2=copy_lambda;
    copy_lambda2();
}
//C++20支持lambda捕获结构化绑定
std::map<int,std::string> m{{1,"one"},{2,"two"}};
void test17(){
    auto [iter,success]=m.insert({3,"three"});
    auto print_success=[success](){
        if(success){
            std::cout<<"insert success"<<std::endl;
        }
        else{
            std::cout<<"insert failed"<<std::endl;
        }
    };
    print_success();
}
//值捕获当前对象的副本(*this)
struct MyStruct{
    int value{10};
    auto get_value_copy_lambda(){
        return [*this](){
            return value;
        };
    }
};
void test18(){
    MyStruct s;
    std::cout<<s.get_value_copy_lambda()()<<std::endl;
}
//lambda中使用可变参数
auto variadic_lambda=[]<typename... Args>(Args... args){
    ((std::cout<<"["<<args<<"]"),...);
    std::cout<<std::endl;
};
void test19(){
    variadic_lambda("1",2,3,4,"5");
    variadic_lambda();
}

int main(){
    //test1();
    //test2();
    //test3();
    //test4();
    //test5();
    //test6();
    //test7();
    //test8();
    //test9();
    //test10();
    //test11();
    //test12();
    //test13();
    //test14();
    //test15();
    //test16();
    //test17();
    //test18();
    test19();

    return 0;
}