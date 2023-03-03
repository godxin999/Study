#pragma once
#include "Disc_quote.h"

class Bulk_quote :public Disc_quote {//类派生列表(冒号后接逗号分隔的基类列表，其中每个基类前可以有访问说明符)
public:
	Bulk_quote() = default;
	Bulk_quote(const std::string&, double, std::size_t, double);
	double net_price(std::size_t)const override;//override显式注明将使用该成员函数改写基类的虚函数
private:
	std::size_t min_qty = 0;//适用折扣政策的最低购买量
	double discount = 0.0;//折扣
};