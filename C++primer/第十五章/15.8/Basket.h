#pragma once
#include "Bulk_quote.h"

class Basket {
public:
	//Basket类使用默认构造函数和拷贝控制成员
	/*
	void add_item(const std::shared_ptr<Quote>& sale) {
		items.insert(sale);
	}
	*/
	void add_item(const Quote& sale) {//将书籍的智能指针加入到basket中
		items.insert(std::shared_ptr<Quote>(sale.clone()));
	}
	void add_item(Quote&& sale) {
		items.insert(std::shared_ptr<Quote>(std::move(sale).clone()));
	}
	double total_receipt(std::ostream&)const;//打印每本书的单价并返回和所有书的总价

private:
	static bool compare(const std::shared_ptr<Quote>& lhs, const std::shared_ptr<Quote>& rhs) {//自定义的比较规则
		return lhs->isbn() < rhs->isbn();
	}
	std::multiset<std::shared_ptr<Quote>, decltype(compare)*> items{compare};//存放多个报价，并按自定义规则排序
};