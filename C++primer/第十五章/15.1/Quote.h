#include <iostream>
#include <algorithm>
#include <cstring>

class Quote {
public:
	std::string isbn()const;
	virtual double net_price(std::size_t n)const;//virtual关键字可以让派生类各自定义适合自身的版本的函数
};