#include <iostream>
#include <cstring>

class Sales_data {
	friend std::istream& read(std::istream&, Sales_data&);
	friend std::ostream& print(std::ostream&, const Sales_data&);
public:
	//Sales_data() = default;
	/*
	Sales_data(std::string s = "") :bookNo(s) {}//等同于默认构造函数
	Sales_data(const std::string& s) :bookNo(s) {}
	Sales_data(const std::string& s, unsigned cnt, double price) :bookNo(s), units_sold(cnt), revenue(price* cnt) {}
	Sales_data(std::istream&);
	*/
	Sales_data(const std::string& s, unsigned cnt, double price) :bookNo(s), units_sold(cnt), revenue(price* cnt) {}
	Sales_data() :Sales_data("", 0, 0) {}//使用委托构造函数
	/*
	Sales_data(std::string s) :Sales_data(s, 0, 0) {}//使用委托构造函数
	Sales_data(std::istream& is) :Sales_data() {//也是委托构造函数，首先构造处对象，然后通过函数体中的read读入数据
		read(is, *this);
	}
	*/
	explicit Sales_data(std::string s) :Sales_data(s, 0, 0) {}//explicit关键字只在类中声明，类外不用重复声明，且只对一个实参的构造函数有效
	explicit Sales_data(std::istream& is) :Sales_data() {
		read(is, *this);
	}
	std::string isbn()const { return bookNo; }
	Sales_data& combine(const Sales_data&);
private:
	double avg_price()const;
	std::string bookNo;
	unsigned units_sold = 0;
	double revenue=0.0;

};

std::ostream& print(std::ostream&, const Sales_data&);
std::istream& read(std::istream&, Sales_data&);