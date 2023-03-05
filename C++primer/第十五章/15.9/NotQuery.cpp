#include "NotQuery.h"
using namespace std;

Query operator~(const Query& operand) {
	return std::shared_ptr<Query_base>(new NotQuery(operand));//会隐式调用Query的接受一个智能指针的构造函数
}

QueryResult NotQuery::eval(const TextQuery& text)const {
	auto result = query.eval(text);
	auto ret_lines = make_shared<set<line_no>>();
	auto beg = result.begin(), end = result.end();
	auto sz = result.get_file()->size();
	for (auto n = 0; n != sz; ++n) {
		if (beg == end || *beg != n) {
			ret_lines->insert(n);
		}
		else if (beg != end) {
			++beg;
		}
	}
	return QueryResult(rep(), ret_lines, result.get_file());
}