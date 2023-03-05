#pragma once
#include "QueryResult.h"

class TextQuery {
public:
	//using line_no = std::vector<std::string>::size_type;//行号
	using line_no = QueryResult::line_no;
	TextQuery(std::ifstream&);//通过文件构造
	QueryResult query(const std::string&)const;//查询函数
private:
	std::shared_ptr<std::vector<std::string>> file;//输入文件
	std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;//每个单词到其行号的映射
};