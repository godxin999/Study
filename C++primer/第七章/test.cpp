/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/1/30 13:24:51
 ************************************************************************/

#include "Screen.h"
#include "Sales_data.h"
#include <unordered_set>

template <class T> class std::hash;//友元声明需要该前置声明

int main(){
	
	std::string null_book = "9-999-99999-9";
	Sales_data item;
	//item.combine(null_book);//合法，string通过转换构造函数隐式转换为了Sales_data类型
	//item.combine("9-999-99999-9");//错误，编译器只能进行一步类型转换
	//item.combine(std::string("9-999-99999-9"));//合法，显式转换为string，隐式转换为Sales_data类型
	//item.combine(Sales_data("9-999-99999-9"));//合法，隐式转换为string，显式转换为Sales_data类型
	//item.combine(std::cin);//合法，通过标准输入创建Sales_data类型，但是combine结束后就被丢弃
	//当这两种构造函数声明为explicit之后
	//item.combine(null_book);//非法，不能执行隐式转换
	//item.combine(std::cin);//
	Sales_data item1(null_book);//explicit构造函数只能用于直接初始化
	//Sales_data item2 = null_book;//explicit构造函数不可用于拷贝初始化
	item.combine(Sales_data(null_book));//显式执行转换
	//item.combine(static_cast<Sales_data>(std::cin));//显式通过istream类型转换weiSales_data类型
	std::unordered_set<Sales_data> s;
	s.insert(item1);
	std::cout << *s.cbegin() <<std::endl;
}

