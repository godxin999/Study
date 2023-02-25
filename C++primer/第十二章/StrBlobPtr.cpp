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
