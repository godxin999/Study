#include "StrBlobPtr.h"
using namespace std;

shared_ptr <vector<string>> StrBlobPtr::check(size_t i, const string& msg)const {
	auto ret = wptr.lock();
	if (!ret) {//如果vector已经被销毁，抛出re异常
		throw runtime_error("unbound StrBlobPtr");
	}
	if (i >= ret->size()) {//如果越界则抛出越界异常
		throw out_of_range(msg);
	}
	return ret;
}

string& StrBlobPtr::deref()const {
	auto p = check(curr, "dereference past end");
	return (*p)[curr];//(*p)是对象所指的vector
}

StrBlobPtr& StrBlobPtr::incr() {
	check(curr, "increment past end of StrBlobPtr");
	++curr;
	return *this;
}

StrBlobPtr& StrBlobPtr::operator++() {
	check(curr, "increment past end of StrBlobPtr");//检查curr是否已经指向尾后位置
	++curr;
	return *this;
}

StrBlobPtr& StrBlobPtr::operator--() {
	check(curr, "decrement past begin of StrBlobPtr");//检查curr是否是首位置，防止产生无效下标
	--curr;
	return *this;
}

StrBlobPtr StrBlobPtr::operator++(int) {
	//此处不需要检查，调用前置递增运算时才需要检查
	StrBlobPtr ret = *this;//记录当前值
	++*this;
	return ret;//返回之前记录的状态
}

StrBlobPtr StrBlobPtr::operator--(int) {
	StrBlobPtr ret = *this;
	--*this;
	return ret;
}