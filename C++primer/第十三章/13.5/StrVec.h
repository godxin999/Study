#pragma once
#include <iostream>
#include <algorithm>
#include <cstring>
#include <memory>

class StrVec {
public:
	StrVec() :elements(nullptr), first_free(nullptr), cap(nullptr) {}
	StrVec(const StrVec&);
	StrVec(StrVec&&)noexcept;//移动构造函数
	StrVec& operator=(const StrVec&);
	StrVec& operator=(StrVec&&)noexcept;
	~StrVec();
	void push_back(const std::string&);
	void push_back(std::string&&);
	size_t size()const {
		return first_free - elements;
	}
	size_t capacity()const {
		return cap - elements;
	}
	std::string* begin()const {
		return elements;
	}
	std::string* end()const {
		return first_free;
	}

private:
	static std::allocator<std::string> alloc;//分配空间
	void chk_n_alloc() {
		if (size() == capacity())reallocate();
	}
	std::pair<std::string*, std::string*> alloc_n_copy(const std::string*, const std::string*);
	void free();//销毁并释放内存
	void reallocate();//获得更多内存并拷贝已有元素
	std::string* elements;//指向数组首元素的指针
	std::string* first_free;//指向数组第一个空闲元素的指针
	std::string* cap;//指向数组尾后位置的指针
};