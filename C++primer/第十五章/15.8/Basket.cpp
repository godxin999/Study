#include "Basket.h"
using namespace std;

double Basket::total_receipt(ostream& os)const {
	double sum = 0.0;
	for (auto it = items.cbegin(); it != items.cend(); it = items.upper_bound(*it)) {//upper_bound返回所有和it关键字相等的元素的尾后位置，即返回指向下一本书的迭代器
		sum+=print_total(os,**it,items.count(*it));//解引用迭代器得到只能指针，再次解引用智能指针得到Quote对象，调用count函数可以得到相同的书(ISBN相同)的数量
	}
	os << "Total Sale: " << sum << endl;
	return sum;
}

