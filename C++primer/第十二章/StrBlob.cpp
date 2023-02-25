#include "StrBlob.h"
#include "StrBlobPtr.h"
using namespace std;
StrBlob::StrBlob() :data(make_shared<vector<string> >()) {

}

StrBlob::StrBlob(initializer_list<string> il) :data(make_shared<vector<string> >(il)) {//使用初始化列表来初始化数据

}

void StrBlob::check(size_type i, const string& msg) const{
	if (i >= data->size()) {
		throw out_of_range(msg);
	}
}

string& StrBlob::front() {
	check(0, "front on StrBlob");
	return data->front();
}

const string& StrBlob::front() const{
	check(0, "front on StrBlob");
	return data->front();
}

string& StrBlob::back() {
	check(0, "back on StrBlob");
	return data->back();
}

const string& StrBlob::back() const {
	check(0, "back on StrBlob");
	return data->back();
}

void StrBlob::pop_back() {
	check(0, "pop_back on empty StrBlob");
	data->pop_back();
}

StrBlobPtr StrBlob::begin() {
	return StrBlobPtr(*this);
}

StrBlobPtr StrBlob::end() {
	auto ret = StrBlobPtr(*this, data->size());
	return ret;
}