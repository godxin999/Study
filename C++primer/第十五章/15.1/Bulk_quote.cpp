#include "Quote.h"

class Bulk_quote :public Quote {//类派生列表(冒号后接逗号分隔的基类列表，其中每个基类前可以有访问说明符)
public:
	double net_price(std::size_t)const override;//override显式注明将使用该成员函数改写基类的虚函数
};