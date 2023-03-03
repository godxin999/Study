#pragma once
#include "Quote.h"

class Disc_quote :public Quote {//定义了纯虚函数的类是抽象基类，不能创建抽象基类的对象
public:
	Disc_quote() = default;
	Disc_quote(const std::string& book, double price, std::size_t qty, double disc) :Quote(book, price), quantity(qty), discount(disc) {}
	double net_price(std::size_t)const = 0;//定义为纯虚函数，=0只能出现在类内部的虚函数声明语句处，表示这个函数是没有实际意义的，无需定义
protected:
	std::size_t quantity = 0;//折扣适用的购买量
	double discount = 0;//折扣
};