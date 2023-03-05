#pragma once
#include "Query.h"

class WordQuery :public Query_base {
	friend class Query;//定义友元以访问私有成员
	WordQuery(const std::string& s) :query_word(s) {}
	QueryResult eval(const TextQuery& t)const {//定义继承来的纯虚函数
		return t.query(query_word);
	}
	std::string rep()const {
		return query_word;
	}
	std::string query_word;//要查询的单词
};