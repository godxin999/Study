#pragma once
#include <iostream>
#include <algorithm>
#include <cstring>
#include <memory>
#include <set>

class Quote {
public:
	Quote() = default;
	Quote(const Quote&) = default;
	Quote(Quote&&) = default;
	Quote& operator=(const Quote&) = default;
	Quote& operator=(Quote&&) = default;
	Quote(const std::string& book, double sales_price) :bookNo(book), price(sales_price) {}
	virtual ~Quote() = default;
	std::string isbn()const {
		return bookNo;
	}
	virtual double net_price(std::size_t n)const {//virtual关键字可以让派生类各自定义适合自身的版本的函数
		return n * price;
	}
	virtual Quote* clone()const& {//为了将不同书籍加入basket，所以定义虚clone函数，各自对应左值和右值
		return new Quote(*this);
	}
	virtual Quote* clone()&& {
		return new Quote(std::move(*this));
	}
private:
	std::string bookNo;
protected:
	double price = 0.0;
};

double print_total(std::ostream&, const Quote&, std::size_t);