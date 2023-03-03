#include "Bulk_quote.h"
using namespace std;

Bulk_quote::Bulk_quote(const string& book, double p, size_t qty, double disc) :Disc_quote(book, p, qty, disc) {}

double Bulk_quote::net_price(size_t cnt) const {
	if (cnt >= min_qty) {
		return cnt * (1 - discount) * price;//派生类可以访问基类的公有成员和受保护成员
	}
	else {
		return cnt * price;
	}

}