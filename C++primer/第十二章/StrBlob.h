#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <memory>

class StrBlob {
public:
	using size_type = std::vector<std::string>::size_type;
	StrBlob();
	StrBlob(std::initializer_list<std::string> il);
	size_type size()const { return data->size(); }
	bool empty()const { return data->empty(); }
	void push_back(const std::string& t) { data->push_back(t); }
	void pop_back();
	std::string& front();
	const std::string& front()const;
	std::string& back();
	const std::string& back()const;
private:
	std::shared_ptr<std::vector<std::string> > data;
	void check(size_type i, const std::string& msg) const;
};