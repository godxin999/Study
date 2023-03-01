#pragma once
#include "StrBlob.h"

class StrBlobPtr{
public:
	StrBlobPtr() :curr(0) {}
	StrBlobPtr(StrBlob& a, size_t sz = 0) :wptr(a.data), curr(sz) {}
	std::string& deref()const;
	StrBlobPtr& incr();//前缀递增
	StrBlobPtr& operator++();//重载前缀递增
	StrBlobPtr& operator--();//重载前缀递减
	StrBlobPtr operator++(int);//重载后缀递增
	StrBlobPtr operator--(int);//重载后缀递减
	std::string& operator*()const {
		auto p = check(curr, "dereference past end");
		return (*p)[curr];
	}
	std::string* operator->()const {
		//把实际工作委托给解引用运算符
		return &this->operator*();
	}

private:
	std::shared_ptr<std::vector<std::string> > check(std::size_t, const std::string&)const;
	std::weak_ptr<std::vector<std::string> > wptr;
	std::size_t curr;//数组中的当前位置
};