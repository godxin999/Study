/*************************************************************************
	> File Name: 32.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/22 9:46:43
 ************************************************************************/

#include <iostream>
#include <yvals_core.h>
//__has_include用于判断某个头文件是否能被包含进来
#if __has_include(<optional>)
#include <optional>
#define have_optional 1
#elif __has_include(<experimental/optional>)
#include <experimental/optional>
#define have_optional 1
#define experimental_optional 1
#else
#define have_optional 0
#endif
//__has_cpp_attribute用于判断某个属性是否可用
void test1(){
    std::cout<<__has_cpp_attribute(no_unique_address)<<std::endl;
}
//语言功能特性测试宏
void test2(){
    std::cout<<__cpp_decltype<<std::endl;
}
//标准库功能特性测试宏
void test3(){
    std::cout<<__cpp_lib_concepts<<std::endl;
}
//C++20VA_OPT宏告诉编译器参数是可选的
#define LOG(msg,...) printf("[" __FILE__ ":%d] " msg, __LINE__, ##__VA_ARGS__)
#define LOG2(msg,...) printf("[" __FILE__ ":%d] " msg, __LINE__ __VA_OPT__ (,) __VA_ARGS__)
void test4(){
    LOG("hello world\n");
    LOG("hello %d\n",2024);
    LOG2("hello world\n");
    LOG2("hello %d\n",2024);
}

int main(){
    //test1();
    //test2();
    //test3();
    test4();

    return 0;
}