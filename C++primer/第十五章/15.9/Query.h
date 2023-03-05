#pragma once
#include "Query_base.h"
#include "WordQuery.h"

class Query {
	friend Query operator~(const Query&);
	friend Query operator|(const Query&, const Query&);
	friend Query operator&(const Query&, const Query&);
public:
	Query(const std::string& s) :q(new WordQuery(s)) {}//构建一个新的WordQuery
	//接口函数，调用对应的Query_base操作
	QueryResult eval(const TextQuery& t)const {
		return q->eval(t);
	}
	std::string rep()const {
		return q->rep();
	}
	friend std::ostream& operator<<(std::ostream&, const Query&);
private:
	Query(std::shared_ptr<Query_base> query) :q(query) {}
	std::shared_ptr<Query_base> q;
};