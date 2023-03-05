#pragma once
#include "Query.h"

class BinaryQuery :public Query_base {//BinaryQuery也是一个抽象基类，用于保存操作两个运算对象的查询类型所需的数据
protected:
	BinaryQuery(const Query& l, const Query& r, std::string s) :lhs(l), rhs(r), opSym(s) {}
	//继承了eval纯虚函数，本身为一个抽象基类
	std::string rep()const {
		return "(" + lhs.rep() + " " + opSym + " " + rhs.rep() + ")";
	}
	Query lhs, rhs;//左侧和右侧的运算对象
	std::string opSym;//运算符的名字
};