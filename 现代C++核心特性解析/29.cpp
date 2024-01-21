/*************************************************************************
	> File Name: 29.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/20 21:54:39
 ************************************************************************/

#include <iostream>
#include <string>
//16进制字面量
void test1(){
    //0x1.7p+2表示(0x1.7)*2^2，p后面的数字表示指数，指数使用十进制表示，前面的是尾数，尾数使用十六进制表示
    double float_arr[]{5.876,1000,0.117};
    for(auto elem: float_arr){
        //hexfloat以十六进制浮点数的形式输出浮点数
        std::cout<<std::hexfloat<<elem<<std::endl;
    }
    double floar_arr2[]{0x1.7p+2,0x1df3b64p-4,0x1f4p+9};
    for(auto elem: floar_arr2){
        //defaultfloat恢复默认的浮点数输出格式
        std::cout<<std::defaultfloat<<elem<<std::endl;
    }
}
//2进制字面量
void test2(){
    auto x=0b110101010L+0xcdL+077LL+42;
    std::cout<<x<<std::endl;
}
//单引号作为整数分隔符
void test3(){
    constexpr int x=123'456;
    static_assert(x==0x1e'240);
    static_assert(x==036'11'00);
    static_assert(x==0b11'110'001'001'000'000);
}
//使用R声明原生字符串字面值
void test4(){
    //prefix R"delimiter(raw_characters)delimiter"
    //其中prefix是可选的前缀，delimiter是可选的定界符，raw_characters是对应的原生字符串
    //delimiter是长度最大为16的字符串，其不能包含括号、反斜杠、空格
    //原生字符串即所见即所得，不会对字符串中的字符进行转义
    char hello_world_html[]=R"cpp(<!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=yes">
        <title>Hello World!</title>
    </head>
    <body>
    Hello World!
    </body>
    </html>)cpp";
    //prefix有u8、u、U和L
    //u8表示utf-8编码的字符串字面值
    //u表示utf-16编码的字符串字面值
    //U表示utf-32编码的字符串字面值
    //L表示宽字符编码的字符串字面值
    char8_t utf8[]=u8R"(你好世界)";
    char16_t utf16[]=uR"(你好世界)";
    char32_t utf32[]=UR"(你好世界)";
    wchar_t wide[]=LR"(你好世界)";
    std::cout<<hello_world_html<<std::endl;
}
//用户自定义字面量
unsigned long long operator"" _kib(unsigned long long kib){
    return kib*1024;
}
unsigned long long operator"" _mib(unsigned long long mib){
    return mib*1024*1024;
}
void test5(){
    auto x=4_kib+2_mib;
    std::cout<<x<<std::endl;
}
//可变参数模板自定义字面量
template <char... c>
std::string operator"" _s(){
    std::string str;
    (str.push_back(c),...);
    return str;
}
void test6(){
    auto str=123_s;
    std::cout<<str<<std::endl;

}

int main(){
    //test1();
    //test2();
    //test3();
    //test4();
    //test5();
    test6();

    return 0;
}