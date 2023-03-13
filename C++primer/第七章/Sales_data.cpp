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

std::istream& operator>>(std::istream& is, Sales_data& item) {
	double price = 0;
	is >> item.bookNo >> item.units_sold >> price;
	if (is) {//输入运算符必须处理输入可能失败的情况
		item.revenue = price * item.units_sold;
	}
	else {
		item = Sales_data();//输入失败就为对象赋予默认的状态
	}
	return is;
}

std::ostream& print(std::ostream& os, const Sales_data& item) {
	os << item.isbn() << " " << item.units_sold << " " << item.revenue << " " << item.avg_price();
	return os;
}

std::ostream& operator<<(std::ostream& os, const Sales_data& item) {
	os << item.isbn() << " " << item.units_sold << " " << item.revenue << " " << item.avg_price();
	return os;
}

Sales_data& Sales_data::operator+=(const Sales_data& rhs) {
	units_sold += rhs.units_sold;
	revenue += rhs.revenue;
	return *this;
}

Sales_data operator+(const Sales_data& lhs, Sales_data& rhs) {
	Sales_data sum = lhs;
	sum += rhs;
	return sum;
}
bool operator==(const Sales_data& lhs, const Sales_data& rhs) {
	return lhs.isbn() == rhs.isbn() && lhs.units_sold == rhs.units_sold && lhs.revenue == rhs.revenue;
}
bool operator!=(const Sales_data& lhs, const Sales_data& rhs) {
	return !(lhs == rhs);
}

namespace std {//实现调用函数
	size_t hash<Sales_data>::operator()(const Sales_data& s)const {
		return hash<string>() (s.bookNo) ^
			hash<unsigned>() (s.units_sold) ^
			hash<double>() (s.revenue);
	}
}//命名空间右花括号后没有分号