/*************************************************************************
	> File Name: Sales_data.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/4 12:46:47
 ************************************************************************/

#include "Sales_data.h"
/*
Sales_data::Sales_data(std::istream& is) {
	read(is, *this);
}
*/
Sales_data& Sales_data::combine(const Sales_data& rhs) {//参数为常量引用，所以可以传入一个临时量作为参数
	units_sold += rhs.units_sold;
	revenue += rhs.revenue;
	return *this;
}

double Sales_data::avg_price()const {
	if (units_sold)return revenue / units_sold;
	else return 0;
}

std::istream& read(std::istream& is, Sales_data& item) {
	double price = 0;
	is >> item.bookNo >> item.units_sold >> price;
	item.revenue = price * item.units_sold;
	return is;
}

std::ostream& print(std::ostream& os, const Sales_data& item) {
	os << item.isbn() << " " << item.units_sold << " " << item.revenue << " " << item.avg_price();
	return os;
}