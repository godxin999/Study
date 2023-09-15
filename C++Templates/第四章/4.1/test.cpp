/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/15 21:31:38
 ************************************************************************/

#include <iostream>
/*
void print(){
    //为了结束递归定义不接受参数的非模板函数print
}
*/
//定义接受单个参数的print也可以结束递归
template <typename T>
void print(T arg){
    std::cout<<arg<<std::endl;
}

template <typename T,typename... Args>
void print(T firstArg,Args... args){
   std::cout<<firstArg<<std::endl;
   std::cout<<sizeof...(Args)<<std::endl;//打印剩余参数的个数
   std::cout<<sizeof...(args)<<std::endl;//打印剩余参数的个数
   print(args...); 
}

int main(){
    std::string s("world");
    print(7.5,"hello",s); 



    return 0;
}
