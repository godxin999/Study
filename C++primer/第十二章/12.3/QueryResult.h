#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <memory>

class QueryResult {
	friend std::ostream& print(std::ostream&, const QueryResult&);//打印函数
public:
	using line_no = std::vector<std::string>::size_type;//行号
	QueryResult(std::string s, std::shared_ptr<std::set<line_no>> p, std::shared_ptr<std::vector<std::string>> f) :sought(s), lines(p), file(f) {}//通过构造函数将将数据保存在私有对象中
	std::set<line_no>::const_iterator begin() {
		return lines->cbegin();
	}
	std::set<line_no>::const_iterator end() {
		return lines->cend();
	}
	std::shared_ptr<std::vector<std::string>> get_file() {
		return file;
	}
private:
	std::string sought;//单词
	std::shared_ptr<std::set<line_no>> lines;
	std::shared_ptr<std::vector<std::string>> file;
};
