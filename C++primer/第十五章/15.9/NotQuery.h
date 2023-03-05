#pragma once
#include "Query.h"

class NotQuery :public Query_base {
	friend Query operator~(const Query&);
	NotQuery(const Query& q) :query(q) {}
	QueryResult eval(const TextQuery&)const;
	std::string rep()const {
		return "~(" + query.rep() + ")";
	}
	Query query;
};

